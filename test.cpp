#include "rdb.h"
using namespace std;

int main () {

    vector<string> attrnames = {"Sno", "Name", "Roll_no", "Salary"};
    vector<int> attrtypes = {0, 2, 0, 1};
    Relation R(attrnames, attrtypes);

    for( int i = 0; i < 5; ++i) {
        integerAttr *n = new integerAttr(i+1);
        integerAttr *r = new integerAttr(i+5*783);
        stringAttr *s = new stringAttr("alpha");
        floatAttr *f = new floatAttr(988*5+i*i/7);

        vector<Attr *> V = {n, s, r, f};

        Record *rec = new Record(V);

        R.addRecord(rec);
    }

    Relation R1(attrnames, attrtypes);

    for( int i = 0; i < 5; ++i) {
        integerAttr *n = new integerAttr(i+1);
        integerAttr *r = new integerAttr(i+5*783);
        stringAttr *s = new stringAttr("beta");
        floatAttr *f = new floatAttr(988*5+i*i/7);

        vector<Attr *> V = {n, s, r, f};

        Record *rec = new Record(V);

        R1.addRecord(rec);
    }

    cout << "R\n";
    R.print();
    cout << "R1\n";
    R1.print();

    cout << "UNION\n";
    Relation *union_r = Union (&R, &R1);
    union_r->print();

    cout << "Projection\n";
    list<string> L = {"Name", "Salary", "Sno"};
    Relation *project_r = Projection (union_r, L);
    project_r->print();

    // cout << "Rename\n";
    // Relation *rename_r = Difference (union_r, "Roll_no", "RollNumber");
    // rename_r->print();

    // cout << "Cartesian\n";
    // Relation *cartesian_r = CartesianProduct (&R, &R1);
    // cartesian_r->print();

    DNFformula f;
    integerAttr two(2);
    integerAttr five(5);
    stringAttr alpha("alpha");
    list<list<tuple<string, char, Attr *>>> ops = {{make_tuple("Sno", '>', &two), make_tuple("Sno", '<', &five), make_tuple("Name", '=', &alpha)}};
    f.ops = ops;
    cout << "Select\n";
    Relation *select_r = Union (union_r, &f);
    select_r->print();

    cout << "Join\n";
    list<string> joinattr = {"Sno"};
    Relation *join_r = naturaljoin (&R, &R1, joinattr);
    join_r->print();

    // if (!union_r) delete union_r;

    return 0;
}