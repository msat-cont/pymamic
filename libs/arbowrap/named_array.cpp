//---------------------------------------------------------------------------
// named_array.cpp - Implementation of the User Layer
//
// In this file we have the implementation of TNamedArray::Serialize(),
// TNamedArray::Unserialize() and an output operator for TNamedArray (which is not required
// by user layer).
//
// Authors: Martin Saturka (martin.saturka@sourcefabric.org)
// Copyright (c) 2014 Sourcefabric o.p.s.
//---------------------------------------------------------------------------
//#pragma hdrstop
#include "named_array.hpp"
//#pragma package(smart_init)

//---------------------------------------------------------------------------
// Class TNamedArray
//---------------------------------------------------------------------------
/**
* Returns the serialized version of this object.
* This method is required  by  stObject interface.
* @warning If you don't know how to serialize an object, this methos may
* be a good example.
*/
const stByte * TNamedArray::Serialize()
{
    if (Serialized == NULL)
    {
        Serialized = new stByte[GetSerializedSize()];
        memcpy(Serialized, &Length, sizeof(unsigned));

        memcpy(Serialized + sizeof(unsigned), Values, Length * sizeof(unsigned));
        Values = (unsigned *) (Serialized + sizeof(unsigned));

        memcpy(Serialized + (sizeof(unsigned) * (Length + 1)), Name.c_str(), Name.length());
    }

    return Serialized;
}//end TNamedArray::Serialize

/**
* Rebuilds a serialized object.
* This method is required  by  stObject interface.
*
* @param data The serialized object.
* @param datasize The size of the serialized object in bytes.
* @warning If you don't know how to serialize an object, this methos may
* be a good example.
*/
void TNamedArray::Unserialize(const stByte *data, stSize datasize){
    if (Serialized != NULL)
    {
        delete [] Serialized;
    }

    Serialized = new stByte[datasize];
    memcpy(Serialized, data, datasize);

    Length = ((unsigned *) Serialized)[0];
    Values = (unsigned *) (Serialized + sizeof(unsigned));

    Name.assign((char *)(Serialized + (sizeof(unsigned) * (Length + 1))), datasize - (sizeof(unsigned) *(Length + 1)));

}//end TNamedArray::Unserialize

//---------------------------------------------------------------------------
// Output operator
//---------------------------------------------------------------------------
/**
* This operator will write a string representation of a name array to an outputstream.
*/
ostream & operator << (ostream & out, TNamedArray & obj)
{
   out << "[Ref=" << obj.GetName() << ";Len=" <<
         obj.GetLength() <<
         "]";
   return out;
}//end operator <<

