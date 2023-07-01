#include "rdb.h"

Relation * naturaljoin(Relation *R1, Relation *R2, list<string> joinattr) {

    // Renaming the attributes in R2, common to R1 and R2
    Relation *renamed_R2 = new Relation (*R2);
    for (auto attrname : joinattr) {
        renamed_R2 = Difference (renamed_R2, attrname, attrname+"_cpy");
    }

    // Taking Cartesian Product of R1 and renamed_R2
    Relation *cartesian_R1_R2 = CartesianProduct (R1, renamed_R2);

    // Selecting records that are to be joined
    Relation *select_R = new Relation (cartesian_R1_R2->getAttrnames(), cartesian_R1_R2->getAttrtypes());

    // Fetching the attribute index of joint attribute in cartesian
    vector<int> keep_idx;
    vector<string> attrnames = select_R->getAttrnames ();
    for (auto attrname : joinattr) {
        for (size_t i = 0; i < attrnames.size(); ++i) {
            if (attrnames[i] == attrname || attrnames[i] == attrname+"_cpy") {
                keep_idx.push_back (i); // this will store attributes in even indices and renamed attributes adjacent to them 
            }
        }
    }

    // filtering only those records where the joint attributes matches both the tables
    for (auto rec : cartesian_R1_R2->getRecords()) {
        vector <Attr *> record = rec->getRecord ();
        bool should_add = false;
        for (size_t i = 0; i < keep_idx.size(); i = i+2) {
            if (*(record[keep_idx[i]]) == *(record[keep_idx[i+1]])) should_add = true;
            else {
                should_add = false;
                break;
            }
        }
        if (should_add) select_R->addRecord (rec);
    }

    // Creating list of projected attributes which excludes renamed attributes
    list<string> project_attr;
    for (auto attrname : attrnames) {
        if (attrname.substr (attrname.length() - 3) == "cpy") continue;
        else project_attr.push_back(attrname);
    }

    Relation *join_R = Projection (select_R, project_attr);
    return join_R;
}