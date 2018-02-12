#ifndef MESSAGEPROTOCOL_SERIALIZABLE_H
#define MESSAGEPROTOCOL_SERIALIZABLE_H

#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/serialization/vector.hpp>
#include <sstream>

template<class DerivedSerializable>
class Serializable {
public:
    friend class boost::serialization::access;

    std::string marshal() {
        std::stringbuf buffer;
        std::ostream os(&buffer);
        boost::archive::binary_oarchive oar(os, boost::archive::no_header);
        DerivedSerializable *ds = static_cast<DerivedSerializable *>(this);
        oar << *ds;
        return buffer.str();
    }

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
