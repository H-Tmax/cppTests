/**
 * @file Sendable.h
 * @author hspark
 * @date 2018-FEB-12
 * @version 1.0
 * @brief Sendable header file for TCP (Tibero Communication Protocol)
 *
 * Any class that inherits the Sendable class can use "tb_sendto" function to send itself to another
 * class that inherits Recievable class.
 */

#ifndef MESSAGEPROTOCOL_SENDABLE_H
#define MESSAGEPROTOCOL_SENDABLE_H

#include "TCP.h"

/**
 * Sendable class which utilizes Curiously Recurring Template Pattern (CRTP) to get the "this" object
 * of the derived class instead of itself.
 *
 * It has only one public function, which is basically "I am transmitting myself to someone" function.
 *
 * This class inherits Serializable class which directly implements BOOST::serialize library.
 *
 * @tparam DerivedSendable derived class that inherits this Sendable class.
 */
template<class DerivedSendable>
class Sendable : public Serializable<DerivedSendable> {
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////PUBLIC API//////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
public:
    /**
     * The only public method of Sendable class.
     *
     * @tparam Recipient Any class that inherits Receivable class.
     * @param receiver any Receivable object
     */
    template<typename Recipient>
    void tb_sendto(Recipient receiver) {
        std::string payload;
        TCPHeader tcpHeader;

        //Serialize "this" object, which is a derived sendable object.
        payload = this->marshal();
        tcpHeader = initializeHeader(payload);

        this->transmitSendable(receiver, tcpHeader, payload);
    }

    /**
     * pure virtual method that NEEDS TO BE OVERRIDEN
     * @return type of the Sendable based on Enum (TCP_TYPE_LIST)
     */
    virtual int getSendableType() = 0;

///////////////////////////////////////////////////////////////////////////////////
//////////////////////////////PRIVATE HELPER METHODS///////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
private:
    /**
     * initializeHeader function for TCP header.
     *
     * @param payload serialized payload in the form of a std::string
     * @return populated TCPHeader object
     */
    TCPHeader initializeHeader(std::string payload) {
        TCPHeader header;
        int payloadSize = payload.length();

        //Sendable Type must be given from a developer.
        header.sendableType = this->getSendableType();
        header.payloadSize = payloadSize;
        /*
         * If the size of (payload + TCPHeader) is bigger than a kernel's pipe buffer size,
         * Pipe does not guarantee the atomicity; therefore, Sendable object needs to be
         * split and sent separately in a size smaller the buffer size respectively.
         */
        header.payloadSplit = payloadSize + sizeof(header) > PIPE_BUF ? true : false;

        return header;
    }

    /**
     * Refactored transmitSendable class which decides which transmitSendable function
     * "this" sendable object should follow through.
     *
     * @tparam Recipient Any class that inherits Receivable
     * @param target Any receivable object
     * @param header TCPHeader that needs to be used for every Sendable object
     * @param payload serialized payload
     */
    template<typename Recipient>
    void transmitSendable(Recipient target, TCPHeader header, std::string payload) {

        int targetFD = target->pipe.getWriteFd();

        if (header.payloadSplit) {
            this->transmitSplitSendable(targetFD, header, payload);
        } else {
            this->transmitSingleSendable(targetFD, header, payload);
        }
    }

    /**
     * A function that actually transmits a sendable object which does not require splitting.
     *
     * @param targetFD Receiver's write FD
     * @param header The same header given from transmitSendable
     * @param payload The same payload given from transmitSendable
     */
    void transmitSingleSendable(int targetFD, TCPHeader header, std::string payload) {

        const byte *finalPayload = (this->fuse(header, payload)).c_str();

        int finalPayloadSize = sizeof(TCPHeader) + payload.length();

        write(targetFD, finalPayload, finalPayloadSize);

        ///////////TODO: this causes SIGSEGV?; Check Valgrind for a possible leak
        //delete reinterpret_cast<const char*>(finalPayload, finalPayloadSize);
    }

    /**
     * A function that fuses a given header and a given payload together
     *
     * @tparam Header Any header type; for now, there is TCPHeader, and PartialHeader (2018-FEB-12).
     * @param header Any header struct
     * @param payload serialized payload in the form of a std::string
     * @return fused & serialized payload in the form of a std::string
     */
    template<typename Header>
    std::string fuse(Header header, std::string payload) {
        int headerSize = sizeof(Header);
        int payloadSize = payload.length();
        int finalPayloadSize = headerSize + payloadSize;

        byte *finalPayload = new char[finalPayloadSize];

        memcpy(finalPayload, &header, headerSize);

        memcpy(finalPayload + headerSize, payload.c_str(), payloadSize);

        return std::string(reinterpret_cast<const char *>(finalPayload), finalPayloadSize);
    }

    /**
     * Overloaded fuse function
     *
     * @tparam Header Any header type; for now, there is TCPHeader, and PartialHeader (2018-FEB-12).
     * @param header Any header struct
     * @param payload serialized payload in the form of a std::string
     * @param payloadBeginIndex An index at which payload is going to be read
     * @return fused & serialized payload in the form of a std::string
     */
    template<typename Header>
    std::string fuse(Header header, std::string payload, int payloadBeginIndex) {
        int headerSize = sizeof(Header);
        int payloadSize = payload.length();
        int finalPayloadSize = headerSize + payloadSize;

        byte *finalPayload = new char[finalPayloadSize];

        memcpy(finalPayload, &header, headerSize);

        memcpy(finalPayload + headerSize, payload.c_str() + payloadBeginIndex, payloadSize);

        return std::string(reinterpret_cast<const char *>(finalPayload), finalPayloadSize);
    }

    /**
     * A function that actually transmits a split sendable object
     *
     * @param targetFD Receiver's write FD
     * @param header The same header given from transmitSendable
     * @param payload The same payload given from transmitSendable
     */
    void transmitSplitSendable(int targetFD, TCPHeader tcpHeader, std::string payload) {

        //Temporarily store total payload size since header will be updated
        int remainingPayloadSize = tcpHeader.payloadSize;

        //Size of the payload that will be split per raw Sendable
        const int splitSize = (PIPE_BUF - sizeof(TCPHeader) - sizeof(PartialHeader));

        //TCPHeader is already set, only needs to update the size of the payload
        tcpHeader.payloadSize = splitSize;

        PartialHeader partialHeader;
        partialHeader = initializeHeader(payload, splitSize);

        while (remainingPayloadSize > 0) {
            //Change the payload size ONLY for the last raw Sendable
            if (tcpHeader.payloadSize < splitSize) {
                tcpHeader.payloadSize = remainingPayloadSize;
            }

            //The index is used to mark the point from which the payload should be read per each iteration
            int index = (partialHeader.sequence - 1) * (tcpHeader.payloadSize);

            //Amalgamate the payload with the partial header first, then attach it to the preset TCPHeader.
            std::string splitPayload = this->fuse(tcpHeader, this->fuse(partialHeader, payload, index));

            //Send the combined result as if it is a single sendable.
            this->transmitSingleSendable(targetFD, tcpHeader, splitPayload);

            //post process
            partialHeader.sequence++;
            remainingPayloadSize -= splitSize;
        }
    }

    /**
     * Overloaded initializeHeader function for partial header.
     *
     * @param payload serialized payload in the form of a std::string
     * @param splitSize size that the payload should be split per splitSendable
     * @return populated PartialHeader object
     */
    PartialHeader initializeHeader(std::string payload, int splitSize) {
        PartialHeader partial;

        //Get a unique ID, then set the sequence number as 1
        partial.ID = this->makeUniqueID();
        partial.sequence = 1;

        int payloadSize = payload.length();

        /*
         * The last splitSendable's payload will be the remainder of the
         * (total payload size / payload size that can be sent per each split sendable)
         */
        int lastPayloadSize = payloadSize % splitSize;

        /*
         * If there is going to be a remainder, final split sendable should be created and sent
         * to take care of the remainder. If the payload can be split evenly into the multiples of a
         * split size, no need to send another sendable.
         */
        if (lastPayloadSize) {
            partial.totalCount = (payloadSize / splitSize) + 1;
        } else {
            partial.totalCount = (payloadSize / splitSize);
        }
    }

    /**
     * Function that creates a unique ID based on the time that sendto function is called, and
     * the pointer address of "this" object.
     *
     * This method, however, does not perfectly guarantee the uniqueness of the ID.
     * It provides statistically "safe enough" number for an optimal performance (I hope)
     *
     * Should consider adding a couple more random "spices," or even changing the mechanism into
     * an entirely new unique ID dispensing system.
     *
     * @return unique long int value
     */
    long int makeUniqueID() {
        //To get a random value based on time
        std::srand(std::time(nullptr));

        //Get two random values to be used as salts
        int salt = std::rand();
        int pepper = std::rand();
        uint64_t pointerAddress = reinterpret_cast<uint64_t>(this);

        //Setup the hash function to be used for the type (long int)
        std::hash<long int> hash;

        //Sprinkle some spices, then brutally hash it away
        return hash(pointerAddress + salt + pepper);
    }
};

#endif //MESSAGEPROTOCOL_SENDABLE_H
