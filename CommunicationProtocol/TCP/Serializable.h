/**
 * @file Serializable.h
 * @author hspark
 * @date 2018-FEB-12
 * @version 1.0
 * @brief Serializable header file
 *
 * Any class that inherits the Serializable class can serialize & deserialize itself.
 * Courtesy of BOOST::serialization library
 */

#ifndef MESSAGEPROTOCOL_SERIALIZABLE_H
#define MESSAGEPROTOCOL_SERIALIZABLE_H

#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/serialization/vector.hpp>

template<class DerivedSerializable>
class Serializable {
public:
    /**
     * BOOST REQUIRED VARIABLE
     */
    friend class boost::serialization::access;

    /**
     * Calling this->marshal will serialize the object, then returns it in a string (byte *)
     *
     * @return serialized "this" object in a form of a std::string
     */
    std::string marshal() {
        std::stringbuf buffer;
        std::ostream os(&buffer);
        boost::archive::binary_oarchive oar(os, boost::archive::no_header);
        DerivedSerializable *ds = static_cast<DerivedSerializable *>(this);
        oar << *ds;
        return buffer.str();
    }

    /**
     * this->unmarshl will deserilize the string input (byte *) and populate the "this" object
     *
     * @param serialized serialized "this" object in a form of a std::string
     */
    void unmarshal(std::string serialized) {
        std::stringbuf buffer;
        buffer.sputn((char *) serialized.c_str(), serialized.length());
        std::istream is(&buffer);
        boost::archive::binary_iarchive iar(is, boost::archive::no_header);
        DerivedSerializable *ds = static_cast<DerivedSerializable *>(this);
        iar >> *ds;
    }
};

#endif //MESSAGEPROTOCOL_SERIALIZABLE_H
