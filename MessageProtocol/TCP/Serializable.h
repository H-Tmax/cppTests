#ifndef MESSAGEPROTOCOL_SERIALIZABLE_H
#define MESSAGEPROTOCOL_SERIALIZABLE_H

#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/serialization/vector.hpp>

template <class DerivedSerializable>
class Serializable {
public:
    friend class boost::serialization::access;

    std::stringbuf buf;
    std::ostream os;
    boost::archive::binary_oarchive oar;

    Serializable() : buf(), os(&buf), oar(os, boost::archive::no_header) {
        //NOTHING TO SEE HERE
    }


    std::string toBytes(){
        DerivedSerializable *ds = static_cast<DerivedSerializable*>(this);
        ds->oar << *ds;
        return ds->buf.str();
    }

    void populateItsOwnData(std::string buf){
        std::stringbuf buffer;
        buffer.sputn((char *)buf.c_str(), buf.length());
        std::istream is(&buffer);
        boost::archive::binary_iarchive iar(is, boost::archive::no_header);
        DerivedSerializable *ds = static_cast<DerivedSerializable*>(this);

        iar >> *ds;
    }


};
#endif //MESSAGEPROTOCOL_SERIALIZABLE_H
