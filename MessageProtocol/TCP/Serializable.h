#ifndef MESSAGEPROTOCOL_SERIALIZABLE_H
#define MESSAGEPROTOCOL_SERIALIZABLE_H


#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/serialization/vector.hpp>

class Serializable {
public:

    friend class boost::serialization::access;


    std::stringbuf buf;
    std::ostream os;
    boost::archive::binary_oarchive oar;

    std::istream is;
    boost::archive::binary_iarchive iar;
    //virtual void serialize() = 0;

    Serializable() : buf(), os(&buf), oar(os, boost::archive::no_header),  is(&buf), iar(is, boost::archive::no_header) {
        //NOTHING TO SEE HERE
    }

//    virtual void serialize(boost::archive::binary_oarchive & oar) = 0;

};
#endif //MESSAGEPROTOCOL_SERIALIZABLE_H
