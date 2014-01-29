//---------------------------------------------------------------------------
// named_array.h - Implementation of the User Layer
//
// This file implements the 2 classes required by the SlimTree Library User
// Layer.
//
// TNamedArray is the object which will be indexed by a metric tree.
// It abstracts a named binary array. Each named binary array has a name,
// and an array of binary values. TNamedBinary defines an interface
// to manipulate its information and also implements the stObject
// interface.
//
// TArrayDistanceEvaluator implements the stMetricEvaluator interface.
// It mesures the distance between two TNamedArray instances.
//
// With these classes, it is possible to define and instantiate any metric tree
// defined by the SlimTree Library.
//
// Authors: Martin Saturka (martin.saturka@sourcefabric.org)
// Copyright (c) 2014 Sourcefabric o.p.s.
//---------------------------------------------------------------------------
#ifndef named_array_H__
#define named_array_H__

#include <math.h>
#include <string>
#include <time.h>
#include <ostream>
#include <assert.h>
using namespace std;

// Metric Tree includes
#include <arboretum/stUserLayerUtil.h>
#include <arboretum/stTypes.h>
#include <arboretum/stUtil.h>

//---------------------------------------------------------------------------
// Class TNamedArray
//---------------------------------------------------------------------------
/**
* This class abstracts a named binary array. Each binary array has a name and
* an array of binary values.
*
* <P>In addition to data manipulation methods (such as GetLength(), GetName()
* and others), this class implements the stObject interface. This interface
* qualifies this object to be indexed by a metric tree implemented by GBDI
* SlimTree Library.
*
* <P>This interface requires no inheritance (because of the use of class
* templates in the Structure Layer) but requires the following methods:
*     - TNamedArray() - A default constructor.
*     - Clone() - Creates a clone of this object.
*     - IsEqual() - Checks if this instance is equal to another.
*     - GetSerializedSize() - Gets the size of the serialized version of this object.
*     - Serialize() - Gets the serialzied version of this object.
*     - Unserialize() - Restores a serialzied object.
*
* <P>Since the array which contains the serialized version of the object must be
* created and destroyed by each object instance, this class will hold this array
* as a buffer of the serialized version of this instance.
*
* <P>The serialized version of the object will be created as follows:<BR>
* <CODE>
* +--------+---------+--------+<BR>
* | Length | 0,1,... | Name[] |<BR>
* +--------+---------+--------+<BR>
* </CODE>
*
* @version 1.0
* @author Martin Saturka
*/
class TNamedArray
{
    public:
        /**
        * Default constructor. It creates a named array with no name and
        * no values. This constructor is required by stObject interface.
        */
        TNamedArray()
        {
            Name = "";
            Length = 0;
            Values = NULL;
        
            // Invalidate Serialized buffer.
            Serialized = NULL;
        }//end TNamedArray

        /**
        * Creates a new named array.
        *
        * @param name The name of the array.
        * @param len Length of the array.
        * @param values Values of the array.
        */
        TNamedArray(const char *name, unsigned len, unsigned *values)
        {
            Name.assign(name);
            Length = len;
            Values = values;

            // Invalidate Serialized buffer.
            Serialized = NULL;
        }//end TNamedArray

        /**
        * Destroys this instance and releases all associated resources.
        */
        ~TNamedArray()
        {
            if (Serialized != NULL)
            {
                delete [] Serialized;
            }//end if
        }//end ~TNamedArray

        /**
        * Gets the length of the named array.
        */
        const unsigned short GetLength() const
        {
            return Length;
        }//end GetLength

        /**
        * Gets values of the named array at the given position.
        */
        const unsigned short GetValues(int ind) const
        {
            return Values[ind];
        }//end GetValues

        /**
        * Gets the name of the array.
        */
        const string & GetName() const
        {
            return Name;
        }//end GetName

        // The following methods are required by the stObject interface.
        /**
        * Creates a perfect clone of this object. This method is required by
        * stObject interface.
        *
        * @return A new instance of TNamedArray wich is a perfect clone
        *  of the original instance.
        */
        TNamedArray * Clone() const
        {
            return new TNamedArray(Name.c_str(), Length, Values);
        }//end Clone

        /**
        * Checks to see if this object is equal to other. This method is required
        * by  stObject interface.
        *
        * @param obj Another instance of TNamedArray.
        * @return True if they are equal or false otherwise.
        */
        bool IsEqual(TNamedArray *obj) const
        {
            if (Length != obj->GetLength())
            {
                return 0;
            }
/*
            for (int i=0; i<Length; i++)
            {
                if (Values[i] != obj->GetValues(i))
                {
                    return 0;
                }
            }

            return 1;
*/
            return (Name == obj->GetName());


        }//end IsEqual

        /**
        * Returns the size of the serialized version of this object in bytes.
        * This method is required  by  stObject interface.
        */
        stSize GetSerializedSize() const
        {
            return (sizeof(unsigned short) + (sizeof(unsigned) * Length)) + Name.length();
        }//end GetSerializedSize

        /**
        * Returns the serialized version of this object.
        * This method is required  by  stObject interface.
        *
        * @warning If you don't know how to serialize an object, this method may
        * be a good example.
        */
        const stByte * Serialize();

        /**
        * Rebuilds a serialized object.
        * This method is required  by  stObject interface.
        *
        * @param data The serialized object.
        * @param datasize The size of the serialized object in bytes.
        * @warning If you don't know how to serialize an object, this method may
        * be a good example.
        */
        void Unserialize (const stByte *data, stSize datasize);
    private:
      /**
      * The name of the array.
      */
      string Name;

      /**
      * Length of the array of values.
      */
      unsigned Length;

      /**
      * Values of the array.
      */
      unsigned * Values;

      /**
      * Serialized version. If NULL, the serialized version is not created.
      */
      stByte * Serialized;
};//end TNamedArray

//---------------------------------------------------------------------------
// Class TNamedArrayDistanceEvaluator
//---------------------------------------------------------------------------
/**
* This class implements a metric evaluator for TNamedArray instances. It calculates
* the distance between cities by performing a euclidean distance between city
* coordinates (I know it is not accurate but is is only a sample!!!).
*
* <P>It implements the stMetricEvaluator interface. As stObject interface, the
* stMetricEvaluator interface requires no inheritance and defines 2 methods:
*     - GetDistance() - Calculates the distance between 2 objects.
*     - GetDistance2()  - Calculates the distance between 2 objects raised by 2.
*
* <P>Both methods are defined due to optmization reasons. Since euclidean
* distance raised by 2 is easier to calculate, It will implement GetDistance2()
* and use it to calculate GetDistance() result.
*
* @version 1.0
* @author Fabio Jun Takada Chino
*/
class TNamedArrayDistanceEvaluator : public stMetricEvaluatorStatistics
{
    public:
        /**
        * Returns the distance between 2 named arrays. This method is required
        * by stMetricEvaluator interface.
        *
        * @param obj1 Object 1.
        * @param obj2 Object 2.
        */
        stDistance GetDistance(TNamedArray *obj1, TNamedArray *obj2)
        {
            assert(obj1->GetLength() == obj2->GetLength());

            int diffs = 0;
#ifdef DEBUG
            UpdateDistanceCount(); // Update Statistics
#endif

            for (int i = 0; i < obj1->GetLength(); i++)
            {
#ifdef __GNUG__
                diffs += __builtin_popcount(obj1->GetValues(i) ^ obj2->GetValues(i));
#else
                unsigned part = obj1->GetValues(i) ^ obj2->GetValues(i);
                for (int shift = 0; shift < sizeof(unsigned); shift++)
                {
                    diffs += part & (1 << shift);
                }
#endif //__GNUG__
            }

            return diffs;
        }//end GetDistance

        /**
        * Returns the distance between 2 named arrays raised by the power of 2.
        * This method is required by stMetricEvaluator interface.
        *
        * @param obj1 Object 1.
        * @param obj2 Object 2.
        */
        stDistance GetDistance2(TNamedArray *obj1, TNamedArray *obj2)
        {
            unsigned dist = GetDistance(obj1, obj2);
            return dist * dist;
        }//end GetDistance2

};//end TTNamedArrayDistanceEvaluator

//---------------------------------------------------------------------------
// Output operator
//---------------------------------------------------------------------------
/**
* This operator will write a string representation of a named array to an outputstream.
*/
ostream & operator << (ostream & out, TNamedArray & obj);

#endif //end named_array_H__
