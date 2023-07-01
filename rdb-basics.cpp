#include "rdb.h"
using namespace std;

// Record Class =============================================================

Record::Record (vector<Attr *> attr_ptr) : attrptr (attr_ptr) {} // ctor
Record::Record (const Record& R) : attrptr (R.attrptr) {} // cctor
Record& Record::operator= (const Record& R) { // cassign
    if (attrptr == R.attrptr) return *this;
    else {
        for (auto attr : attrptr) delete attr; // freeing up memory
        attrptr.clear ();
        attrptr = R.attrptr;
        return *this;
    }
}
Record::~Record () { // dtor
    for (auto attr : attrptr) delete attr; 
    attrptr.clear ();
}

bool Record::operator== (const Record& R) {

    if (attrptr.size() != R.getRecord().size()) return false; // checking if number of values match

    for (size_t i = 0; i < attrptr.size (); ++i) { // checking if each value match each other
        if (*(attrptr[i]) != *(R.getRecord ()[i])) return false;
    }

    return true;
}

vector<Attr *> Record::getRecord () const { return attrptr; }
void Record::setRecord (vector <Attr *> attr_ptr) { attrptr = attr_ptr; }

void Record::print () {
    
    for (size_t i = 0; i < attrptr.size (); ++i) {      
        
        if (attrptr[i]->getType() == 0) {
            integerAttr* int_ptr = dynamic_cast<integerAttr*>(attrptr[i]);
            cout << int_ptr->getValue() << "\t";
        }
        else if (attrptr[i]->getType() == 1) {
            floatAttr* float_ptr = dynamic_cast<floatAttr*>(attrptr[i]);
            cout << float_ptr->getValue() << "\t";
        }
        else if (attrptr[i]->getType() == 2) {
            stringAttr* string_ptr = dynamic_cast<stringAttr*>(attrptr[i]);
            cout << string_ptr->getValue() << "\t";
        }

    }
    cout << endl;
}

// Relation class ============================================================

// CTOR
Relation::Relation (vector<string> attr_names, vector<int> attr_type) :
    nrecs (0),
    attrnames (attr_names),
    attrtypes (attr_type) {
        for (size_t i = 0; i < attrnames.size(); ++i) attrinds.push_back(i+1);
        nattr = attr_names.size ();
    }

// CCTOR
Relation::Relation (const Relation& R) : 
    nattr (R.nattr), 
    nrecs (R.nrecs), 
    attrnames (R.attrnames),
    attrtypes (R.attrtypes),
    attrinds (R.attrinds),
    recs (R.recs) {}

// CASSIGN
Relation& Relation::operator= (const Relation& R) {
    if (recs == R.recs) return *this;
    else {
    // Freeing Up memory
        while(!recs.empty()) delete recs.front(), recs.pop_front();
        recs.clear ();
        attrnames.clear ();
        attrtypes.clear ();
        attrinds.clear ();
        
    // Assigning values
        nattr = R.nattr;
        nrecs = R.nrecs;
        attrnames = R.attrnames;
        attrtypes = R.attrtypes;
        attrinds = R.attrinds;
        recs = R.recs;

        return *this;
    }
}

// DTOR
Relation::~Relation () {
    for (auto rec_ptr : recs) delete rec_ptr;
    recs.clear ();
    attrnames.clear ();
    attrtypes.clear ();
    attrinds.clear ();
}

int Relation::getNAttr () { return nattr; }
int Relation::getNRecs () { return nrecs; }
vector<string> Relation::getAttrnames () { return attrnames; }
vector<int> Relation::getAttrtypes () { return attrtypes; }
list<Record *> Relation::getRecords () { return recs; }

void Relation::setAttrnames (vector<string> R) { attrnames = R; }

void Relation::print () { // Prints the relation in tabular format
    
    // Printing Horizontal Rule
    for (int i = 0; i < nattr*20; ++i) cout << "-";
    cout << endl;

    // Printing Attribute Names
    for (size_t i = 0; i < attrnames.size(); ++i) {
        cout << attrnames[i] << "\t";
    }
    cout << endl;

    // Printing Horizontal Rule
    for (int i = 0; i < nattr*20; ++i) cout << "-";
    cout << endl;

    // Printing record list
    for  (list<Record *>::iterator it = recs.begin(); it != recs.end(); ++it) {
        Record *R = *it;
        R->print ();
    }

    // Printing Horizontal Rule
    for (int i = 0; i < nattr*20; ++i) cout << "-";
    cout << endl;
}

void Relation::addRecord (Record *R) { recs.push_back (R); nrecs++; }
void Relation::deleteRecord (Record *R) {
    for (list<Record *>::iterator it = recs.begin (); it != recs.end (); ++it) {
        if (*it == R) {
            delete *it; // freeing up memory of Record *ptr
            recs.erase (it); // removing pointer to the Record *ptr from list recs 
            nrecs--; // updating number of records
        };
    }
}
void Relation::updateRecord (Record *new_R, Record *old_R) {
    for (list<Record *>::iterator it = recs.begin (); it != recs.end (); ++it) {
        if (*it == old_R) {
            delete *it; // freeing up memory of Record *ptr
            *it = new_R; // updating pointer to the Record *ptr in list recs 
        };
    }
}
bool Relation::containRecord (const Record& R) {
    for (list<Record *>::iterator it = recs.begin (); it != recs.end (); ++it) {
        if (*(*it) == R) return true;
    }
    return false;
}

// BASIC FUNCTIONS ===============================

Relation * Union (Relation *R1, Relation *R2)  { // UNION OPERATOR
    
    // checking if R1 and R2 have same attributes
    if (R1->getNAttr () != R2->getNAttr ()) return nullptr;
    if (R1->getAttrnames () != R2->getAttrnames ()) return nullptr;

    // if R1 and R2 have same attribute names, then perform union
    vector<string> attrnames = R1->getAttrnames ();
    vector<int> attrtypes = R1->getAttrtypes ();
    Relation *R3 = new Relation(attrnames, attrtypes);

    list<Record *> records; // creating a variable for record list
    
    records = R1->getRecords (); // adding records from Relation R1
    for (list<Record *>::iterator it = records.begin (); it != records.end (); ++it) R3->addRecord (*it);

    records = R2->getRecords (); // adding records from Relation R2 ensuring it does not exist in R3 already
    for (list<Record *>::iterator it = records.begin (); it != records.end (); ++it) {
        if (!R3->containRecord (*(*it))) R3->addRecord (*it);
    }

    return R3;
}

Relation * Difference (Relation *R1, Relation *R2) { // DIFFERENCE OPERATOR
    
    // checking if R1 and R2 have same attributes
    if (R1->getNAttr () != R2->getNAttr ()) return nullptr;
    if (R1->getAttrnames () != R2->getAttrnames ()) return nullptr;

    // if R1 and R2 have same attribute names, then perform union
    vector<string> attrnames = R1->getAttrnames ();
    vector<int> attrtypes = R1->getAttrtypes ();
    Relation *R3 = new Relation(attrnames, attrtypes);

    list<Record *> records; // creating a variable for record list
    
    records = R1->getRecords (); // adding records from Relation R1
    for (list<Record *>::iterator it = records.begin (); it != records.end (); ++it) R3->addRecord (*it);

    records = R2->getRecords (); // removing records in R3 that exist in R2
    for (list<Record *>::iterator it = records.begin (); it != records.end (); ++it) {
        if (R3->containRecord (*(*it))) R3->deleteRecord (*it);
    }

    return R3;
}

Relation * CartesianProduct (Relation *R1, Relation *R2) {

    // Creating attrnames vector consisting of all attributes from R1 and R2
    vector<string> attrnames;
    vector<string> attrnames1 = R1->getAttrnames();
    vector<string> attrnames2 = R2->getAttrnames();
    for (auto id: attrnames1) attrnames.push_back(id);
    for (auto id: attrnames2) attrnames.push_back(id);

    // Creating attrtypes vector consisting of all attribute types from R1 and R2
    vector<int> attrtypes;
    vector<int> attrtypes1 = R1->getAttrtypes();
    vector<int> attrtypes2 = R2->getAttrtypes();
    for (auto id: attrtypes1) attrtypes.push_back(id);
    for (auto id: attrtypes2) attrtypes.push_back(id);

    Relation *R3 = new Relation(attrnames, attrtypes); // created a new Relation ptr R3

    list<Record *> R1_recs = R1->getRecords(); // getting records from R1
    list<Record *> R2_recs = R2->getRecords(); // getting records from R2

    // cartesian product : R1 x R2
    for (list<Record *>::iterator it1 = R1_recs.begin (); it1 != R1_recs.end (); ++it1) {
        for (list<Record *>::iterator it2 = R2_recs.begin (); it2 != R2_recs.end (); ++it2) {
            
            // merging records of both the relations
            vector<Attr *> sum_attrptr; // creating a new vector to store attrptr
            vector<Attr *> rec1 = (*it1)->getRecord(); 
            vector<Attr *> rec2 = (*it2)->getRecord();
            for (auto id: rec1) sum_attrptr.push_back(id);
            for (auto id: rec2) sum_attrptr.push_back(id);
            
            Record *R3_rec = new Record(sum_attrptr); // merged record holder
            R3->addRecord (R3_rec); // added the merged record to new relation
        }
    }


    return R3;
}

Relation * Projection (Relation *R, list<string> cols) {

    vector<string> attrnames = R->getAttrnames(); // getting attribute names from relation
    vector<int> attrtypes = R->getAttrtypes(); // getting attribute types from relation
    vector<int> keep_idx; // vector to store index of projected attributes

    for (list<string>::iterator i = cols.begin(); i != cols.end(); ++i) {
        for (size_t j = 0; j < attrnames.size(); ++j) {
            if (attrnames[j] == *i) {
                keep_idx.push_back(j);
                break;
            }
        }
    }

    vector<string> new_attrnames; // created variable to store projected attributes
    vector<int> new_attrtypes; // created variable to store projected attribute types
    for (auto index : keep_idx) {
        new_attrnames.push_back(attrnames[index]);
        new_attrtypes.push_back(attrtypes[index]);
    }

    Relation *Projected_R = new Relation (new_attrnames, new_attrtypes); // created a new relation with projected attributes
    list<Record *> recs = R->getRecords (); // getting the records of relation

    for (auto record : recs) {
        vector<Attr *> projected_attr; // variable to store projected values from record
        for (auto idx : keep_idx) projected_attr.push_back(record->getRecord()[idx]); // copying the required value from record to projection

        Record *projected_rec = new Record (projected_attr); // created projected record
        Projected_R->addRecord (projected_rec); // added projected record to project relation
    }

    return Projected_R;
}

Relation * Union (Relation *R, DNFformula *f) {
    
    vector<string> attrnames = R->getAttrnames(); // getting attributes of the parent relation
    vector<int> attrtypes = R->getAttrtypes(); // getting attribute types of parent relation
    list<Record *> recs = R->getRecords(); // getting list of records from the Relation
    Relation *Select_R = new Relation (attrnames, attrtypes); // creating an empty relation to store filtered records

    for (list<Record *>::iterator rec_ptr = recs.begin(); rec_ptr != recs.end(); ++rec_ptr) { // iterating through each record

        bool select_record = false; // determines selection of record

        // iterating through each clause
        for (list<list<tuple<string, char, Attr *>>>::iterator clause_ptr = f->ops.begin(); clause_ptr != f->ops.end(); ++clause_ptr) {
            
            bool select_clause = true; // determines selection of clause

            // iterating through conditions of each clause
            for (list<tuple<string, char, Attr *>>::iterator tuple_ptr = clause_ptr->begin(); tuple_ptr != clause_ptr->end(); ++tuple_ptr) {
                string attrname = get<0>(*tuple_ptr);
                char op = get<1>(*tuple_ptr);
                Attr *attrptr = get<2>(*tuple_ptr);
                
                if (!checkCondition (R, *rec_ptr, attrname, op, attrptr)) { // checks whether record passes the condition
                    select_clause = false; // if any tuple is false, then conjunction will always be false
                    break;
                }
            }

            if (select_clause) { // if any clause is true, then disjunction will always be true
                select_record = true;
                break;
            }
        }

        if (select_record) Select_R->addRecord (*rec_ptr);
    }

    return Select_R;
}

Relation * Difference (Relation *R, string s1, string s2) {
    
    vector<string> attrnames = R->getAttrnames (); // getting attribute names of Relation

    vector<string> new_attrnames;
    for (size_t i = 0; i < attrnames.size(); ++i) {
        if (attrnames[i] == s1) new_attrnames.push_back(s2);
        else new_attrnames.push_back(attrnames[i]);
    }

    R->setAttrnames (new_attrnames);

    return R;
}

bool checkCondition (Relation *R, Record *rec, string attrname, char op, Attr *attrptr) {
    
    vector<string> attrnames = R->getAttrnames ();
    size_t attrId;
    for (attrId = 0; attrId < attrnames.size(); ++attrId) {
        if (attrnames[attrId] == attrname) break;
    }

    vector<Attr *> record = rec->getRecord();
    
    if (op == '<') return (*(record[attrId]) < *attrptr);
    else if (op == '>') return (*(record[attrId]) > *attrptr);
    else return (*(record[attrId]) == *attrptr);

}