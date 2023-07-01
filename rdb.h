#ifndef __RDB_H
#define __RDB_H

#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <iomanip>
using namespace std;

// Attribute Classes

class Attr {
    public:
        Attr ();
        ~Attr ();

        virtual bool operator== (const Attr&) const = 0;
        virtual bool operator!= (const Attr&) const;
        virtual bool operator< (const Attr&) const = 0;
        virtual bool operator<= (const Attr&) const;
        virtual bool operator> (const Attr&) const;
        virtual bool operator>= (const Attr&) const;

        virtual int getType () const = 0; // return 0: int, 1: float, 2: string
};

// Supported Attribute Types

class integerAttr : public Attr {
    
    private:
        int _value;
    
    public:
        integerAttr (int = 0); //  ctor
        integerAttr (const integerAttr&); // cctor
        integerAttr& operator= (const integerAttr&); // cassign
        ~integerAttr (); //dtor

    // Operator Overloadings
        bool operator== (const Attr& right) const;
        bool operator< (const Attr& right) const;

    // Member Functions
        void setValue (int); // set method
        int getValue () const; // get method
        int getType () const; // returns type of attribute
};

class floatAttr : public Attr {
    
    private:
        float _value;
    
    public:
        floatAttr (float = 0); //  ctor
        floatAttr (const floatAttr&); // cctor
        floatAttr& operator= (const floatAttr&); // cassign
        ~floatAttr (); //dtor

    // Operator Overloadings
        bool operator== (const Attr& right) const;
        bool operator< (const Attr& right) const;

    // Member Functions
        void setValue (float); // set method
        float getValue () const; // get method
        int getType () const; // return type of attribute
};

class stringAttr : public Attr {
    
    private:
        string _value;
    
    public:
        stringAttr (string = ""); // ctor
        stringAttr (const stringAttr&); // cctor
        stringAttr& operator= (const stringAttr&); // cassign
        ~stringAttr (); //dtor

    // Operator Overloadings
        bool operator== (const Attr& right) const;
        bool operator< (const Attr& right) const;

    // Member Functions
        void setValue (string); // set method
        string getValue () const; // get method
        int getType () const; // returns type of attribute
};

// DNF FORMULA

typedef struct DNFformula
{
    list <list <tuple <string, char, Attr *> >> ops;
} DNFformula;

// Record and Relation Classes

class Record {
    
    private:
        vector<Attr *> attrptr;
    
    public:
        Record (vector<Attr *>); // ctor
        Record (const Record&); // cctor
        Record& operator= (const Record&); // cassign
        ~Record (); // dtor

    // Operator Overloading
        bool operator== (const Record&);

    // Member Functions
        void setRecord (vector<Attr *>); // set method
        vector<Attr *> getRecord () const; // get method

        void print (); // Print Record
};

class Relation {
    
    private:
        int nattr, nrecs; // number of attributes and records
        vector<string> attrnames; // schema
        vector<int> attrtypes; // mapping schema to attribute type
        vector<int> attrinds; // mapping schema to indices
        list <Record *> recs; // list of records
    
    public:
        Relation (vector<string>, vector<int>); // ctor
        Relation (const Relation&); // cctor
        Relation& operator= (const Relation&); // cassign
        ~Relation (); // dtor
    
    // Member Functions
        int getNAttr (); // gets number of attributes
        int getNRecs (); // gets number of records
        vector<string> getAttrnames (); // gets attribute names vector
        vector<int> getAttrtypes (); // gets attribute types
        list<Record *> getRecords (); // gets list of records

        void setAttrnames (vector<string>); // sets attribute names

        void print (); // prints the Table in appropriate format
        void addRecord (Record *); // add a new record to the relation
        void deleteRecord (Record *); // delete an existing record from the relation
        void updateRecord (Record *, Record *); // update an existing record in the relation
        bool containRecord (const Record&); // checks existence of record

    // Friend Class
        friend class Record;

    // Friend Functions
        friend Relation * Union (Relation *, Relation *);
        friend Relation * Difference (Relation *, Relation *R2);
        friend Relation * CartesianProduct (Relation *, Relation *);
        friend Relation * projection (Relation *, list<string>);
        friend Relation * Union (Relation *, DNFformula *);
        friend Relation * Difference (Relation *, string, string);

};

Relation * Union (Relation *, Relation *); // union operator
Relation * Difference (Relation *, Relation *); // difference operator
Relation * CartesianProduct (Relation *, Relation *); // cartesian product operation
Relation * Projection (Relation *, list<string>); // projection of new relation with subset of columns

bool checkCondition (Relation *, Record *, string, char, Attr *); // evaluates expression of form (string, char, Attr*). Used in DNF operation
Relation * Union (Relation *, DNFformula *); // select operator

Relation * Difference (Relation *, string, string); // rename attribute name

Relation * naturaljoin(Relation *, Relation *, list<string>); // natural join function

#endif