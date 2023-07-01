#include "rdb.h"
using namespace std;

void displayTableNames (vector< tuple <string, Relation *>> DB) { // display Table Names

    if (DB.size() == 0) {
        cout << "No Tables in Database.\n";
        return;
    }

    cout << "Tables :\n";

    int table_no = 1;
    for (auto table : DB) {
        cout << table_no << " : " << get<0>(table) << endl;
        table_no++;
    }
}

void displayFormula (DNFformula f) {
    for (auto OR : f.ops) {
        for (auto AND : OR) {
            Attr *value = get<2>(AND);
            
            if (value->getType () == 0) {
                integerAttr *int_value = dynamic_cast<integerAttr *>(value);
                cout << "( " << get<0>(AND) << " " << get<1>(AND) << " " << int_value->getValue () << " ) and\n";
            }
            else if (value->getType () == 1) {
                floatAttr *float_value = dynamic_cast<floatAttr *>(value);
                cout << "( " << get<0>(AND) << " " << get<1>(AND) << " " << float_value->getValue () << " ) and\n";
            }
            else if (value->getType () == 0) {
                stringAttr *string_value = dynamic_cast<stringAttr *>(value);
                cout << "( " << get<0>(AND) << " " << get<1>(AND) << " " << string_value->getValue () << " ) and\n";
            }
        }
        cout << "OR" << endl;
    }
}

int main () {

    bool app_status = true;
    
    vector< tuple <string, Relation *>> DB; // database 

    while (app_status) {
        
        int choice;

        cout << "\nWelcome to Relational Database Management\n";
        cout << "\nWhat would you like to do ?\n";
        cout << "\t1 : New Table\n";
        cout << "\t2 : Delete a Table\n";
        cout << "\t3 : Print a Table\n";
        cout << "\t4 : Operate on Tables\n";
        cout << "-1 EXIT ==>\n";
        cout << "\nEnter Choice (1 - 4) : ";
        cin >> choice;

        if (choice == -1) app_status = false; // exit

        else if (choice == 1) { // Creating your table
            string table_name, attr_name;
            int attr_type = -1;
            vector<string> attrnames;
            vector<int> attrtypes;

            cout << "\n-----< CREATING NEW TABLE >-----\n";

            cout << "-1 : BACK ==>" << endl;
            cout << "\nName your Table: "; cin >> table_name; // naming your table
            if (table_name == "-1") continue;

            while (true) {
                
                cout << "\nEnter attribute name (-1 to exit) : "; cin >> attr_name; // reading attribute name
                if (attr_name == "-1") break;

                while (true) { // reading a valid attribute type
                    cout << "Enter attribute type:\n";
                    cout << "1 : integer type\n2 : float type\n3 : string type\n";
                    cout << "Enter choice (1-3): "; cin >> attr_type;
                    if (attr_type != 1 && attr_type != 2 && attr_type != 3) {
                        cout << "Enter a valid choice.\n";
                        continue;
                    }
                    break;
                }
                attrnames.push_back (attr_name); // appending name
                attrtypes.push_back (attr_type-1); // appending type
            }

            if (attr_name == "-1" && attrnames.size() == 0) continue;

            Relation *R = new Relation (attrnames, attrtypes); // creating table
            DB.push_back (make_tuple (table_name, R)); // appending table to database
            cout << "\nNEW TABLE CREATED.\n";
        }

        else if (choice == 2) { // Deleting a table
            int table_no ;
            cout << "\n-----< DELETING A TABLE >-----\n";
            
            displayTableNames (DB);

            cout << "Back ==>" << endl;
            cout << "\nEnter Table number: "; cin >> table_no;

            if (table_no <= 0 || table_no > DB.size()) continue;
            else {
                DB.erase(DB.begin() + table_no - 1);
                cout << "Table Deleted.\n";
            }
        }

        else if (choice == 3) { // Printing a Table
            int table_no = 1;
            cout << "\n-----< PRINTING A TABLE >-----\n";
            
            displayTableNames (DB);

            cout << "Back ==>" << endl;
            cout << "\nEnter Table number: "; cin >> table_no;

            if (table_no <= 0 || table_no > DB.size()) continue;
            else {
                cout << "\n\t=====< Table: " << get<0>(DB[table_no-1]) << " >=====\n" << endl;
                get<1>(DB[table_no-1])->print();
            }
        }

        else if (choice == 4) { // operations on tables
            
            while (true) {
                int option;
                cout << "\n-----< TABLE OPERATIONS >-----\n";

                cout << "\t1 : Union\n";
                cout << "\t2 : Difference\n";
                cout << "\t3 : Cartesian Product\n";
                cout << "\t4 : Natural Join\n";
                cout << "\t5 : Work on a single table\n";
                cout << "-1 BACK ==>\n";

                cout << "\nEnter choice (1-5) : "; cin >> option;

                if (option == -1) break;

                else if (option == 1) { // union of tables
                    
                    int A = -1, B = -1;

                    cout << "\n---< UNION OF TABLES >---\n";

                    displayTableNames (DB);
                    
                    // Taking two valid tables to operate on
                    while (A <= 0 || A > DB.size()) {
                        cout << "Enter Table A (-1 to exit) : "; cin >> A;
                        if (A == -1) break;
                    }
                    while (B <= 0 || B > DB.size()) {
                        cout << "Enter Table B (-1 to exit) : "; cin >> B;
                        if (B == -1) break;
                    }
                    if (A == -1 || B == -1) continue;

                    Relation *union_AB = Union (get<1>(DB[A-1]), get<1>(DB[B-1]));
                    if (!union_AB) {
                        cout << "Union unsuccessful.\n";
                        continue;
                    }

                    string union_table = get<0>(DB[A-1]) + " + " + get<0>(DB[B-1]);
                    DB.push_back (make_tuple (union_table, union_AB));
                    cout << "Union successful.\n";
                }

                else if (option == 2) { // difference of tables
                    
                    int A = -1, B = -1;

                    cout << "\n---< DIFFERENCE OF TABLES >---\n";

                    displayTableNames (DB);
                    
                    // Taking two valid tables to operate on
                    while (A <= 0 || A > DB.size()) {
                        cout << "Enter Table A (-1 to exit) : "; cin >> A;
                        if (A == -1) break;
                    }
                    while (B <= 0 || B > DB.size()) {
                        cout << "Enter Table B (-1 to exit) : "; cin >> B;
                        if (B == -1) break;
                    }
                    if (A == -1 || B == -1) continue;

                    Relation *difference_AB = Difference (get<1>(DB[A-1]), get<1>(DB[B-1]));
                    if (!difference_AB) {
                        cout << "Difference unsuccessful.\n";
                        continue;
                    }

                    string difference_table = get<0>(DB[A-1]) + " - " + get<0>(DB[B-1]);
                    DB.push_back (make_tuple (difference_table, difference_AB));
                    cout << "Difference successful.\n";
                }

                else if (option == 3) { // cartesian of tables
                    
                    int A = -1, B = -1;

                    cout << "\n---< CARTESIAN OF TABLES >---\n";

                    displayTableNames (DB);
                    
                    // Taking two valid tables to operate on
                    while (A <= 0 || A > DB.size()) {
                        cout << "Enter Table A (-1 to exit) : "; cin >> A;
                        if (A == -1) break;
                    }
                    while (B <= 0 || B > DB.size()) {
                        cout << "Enter Table B (-1 to exit) : "; cin >> B;
                        if (B == -1) break;
                    }
                    if (A == -1 || B == -1) continue;

                    Relation *cartesian_AB = CartesianProduct (get<1>(DB[A-1]), get<1>(DB[B-1]));
                    string cartesian_table = get<0>(DB[A-1]) + " x " + get<0>(DB[B-1]);
                    DB.push_back (make_tuple (cartesian_table, cartesian_AB));
                    cout << "Cartesian Product successful.\n";
                }

                else if (option == 4) { // natural join
                    
                    int A = -1, B = -1;

                    cout << "\n---< NATURAL JOIN OF TABLES >---\n";

                    displayTableNames (DB);
                    
                    // Taking two valid tables to operate on
                    while (A <= 0 || A > DB.size()) {
                        cout << "Enter Table A (-1 to exit) : "; cin >> A;
                        if (A == -1) break;
                    }
                    while (B <= 0 || B > DB.size()) {
                        cout << "Enter Table B (-1 to exit) : "; cin >> B;
                        if (B == -1) break;
                    }
                    if (A == -1 || B == -1) continue;
                    
                    string attrname;
                    list<string> joinattr;
                    string joinname;
                    cout << "Enter name of Join Table: "; cin >> joinname;
                    
                    while (true) {
                        cout << "Enter Attribute name (-1 to exit) : ";
                        cin >> attrname;
                        if (attrname == "-1") break;
                        else joinattr.push_back (attrname);
                    }

                    DB.push_back (make_tuple (joinname, naturaljoin (get<1>(DB[A-1]), get<1>(DB[B-1]), joinattr)));
                    cout << "Natural Join successful.\n";
                }

                else if (option == 5) { // working on single table

                    int table_no = -1;

                    cout << "\n---< TABLES >---\n";

                    displayTableNames (DB);
                    
                    // Taking a valid table to operate on
                    while (table_no <= 0 || table_no > DB.size()) {
                        cout << "Enter Table number (-1 to exit) : "; cin >> table_no;
                        if (table_no == -1) break;
                    }

                    string table_name = get<0>(DB[table_no-1]);
                    Relation *table = get<1>(DB[table_no-1]);

                    while (true) {
                        
                        int query;
                        
                        cout << "\nWorking on Table : " << table_name << endl;

                        cout << "\t1 : Add a Record\n"; 
                        cout << "\t2 : Take Projection\n";
                        cout << "\t3 : Perform Selection\n";
                        cout << "\t4 : Rename Attribubte\n";
                        cout << "\t5 : Print Table\n";
                        cout << "-1 BACK ==>\n";

                        cout << "\nEnter Choice (1-5) : "; cin >> query;

                        if (query == -1) break;

                        else if (query == 1) { // add new record
                            vector<string> attrnames = table->getAttrnames ();
                            vector<int> attrtypes = table->getAttrtypes ();

                            vector<Attr *>  attrptr;
                            int n; float f; string s;

                            cout << "Reading Record:\n";
                            for (size_t i = 0; i < attrnames.size(); ++i) {
                                cout << setw (16) << attrnames[i] << " = ";
                                if (attrtypes[i] == 0) {
                                    cin >> n;
                                    attrptr.push_back (new integerAttr (n));
                                }
                                else if (attrtypes[i] == 1) {
                                    cin >> f;
                                    attrptr.push_back (new floatAttr (f));
                                }
                                else if (attrtypes[i] == 2) {
                                    cin >> s;
                                    attrptr.push_back (new stringAttr (s));
                                }
                            }
                            table->addRecord (new Record (attrptr));
                            cout << "Record added successfully.\n";
                        }

                        else if (query == 2) { // Taking Projection
                            string projectname;
                            cout << "Name your Projection Table: "; cin >> projectname;
                            
                            list<string> attrnames;
                            int x;
                            for (auto attrname : table->getAttrnames ()) {
                                cout << attrname << ": Project (0-No/ Yes otherwise) ? = ";
                                cin >> x;
                                if (x == 0) continue;
                                else attrnames.push_back (attrname);
                            }

                            DB.push_back (make_tuple (projectname, Projection (table, attrnames)));
                            cout << "Projection Successful.";
                            
                        }
                        else if (query == 3) { // selection query
                            string selectname;
                            cout << "Name your Selection Table: "; cin >> selectname;

                            DNFformula selectQuery;
                            list<tuple<string, char, Attr *>> andQuery;

                            vector<string> attrnames = table->getAttrnames ();
                            vector<int> attrtypes = table->getAttrtypes ();

                            int n; float f; string s, attrname; char op;
                            
                            while (true) { // constructing DNFformula in interactive manner
                                cout << "Selecting records where:\n";
                                displayFormula (selectQuery);
                                cout << "Enter Attribute name: "; cin >> attrname;
                                cout << "Enter operator (<, =, >) : "; cin >> op;
                                cout << "Enter Value : ";
                                for (size_t i = 0; i < attrnames.size() ; ++i) {
                                    if (attrnames[i] == attrname) {
                                        if (attrtypes[i] == 0) {
                                            cin >> n;
                                            andQuery.push_back (make_tuple (attrname, op, new integerAttr (n)));
                                        }
                                        else if (attrtypes[i] == 1) {
                                            cin >> f;
                                            andQuery.push_back (make_tuple (attrname, op, new floatAttr (f)));
                                        }
                                        else if (attrtypes[i] == 2) {
                                            cin >> s;
                                            andQuery.push_back (make_tuple (attrname, op, new stringAttr (s)));
                                        }
                                    }
                                }
                                int logicChoice;
                                cout << "Logical operator: \n";
                                cout << "\t1 : AND\n";
                                cout << "\t2 : OR\n";
                                cout << ">> Select now >>\n";
                                cout << "Enter choice (1 / 2): "; cin >> logicChoice;

                                if (logicChoice == 1) {
                                    continue;
                                }
                                else if (logicChoice == 2) {
                                    selectQuery.ops.push_back (andQuery);
                                    andQuery.clear();
                                    continue;
                                }
                                else {
                                    selectQuery.ops.push_back (andQuery);
                                    break;
                                }
                            }
                            DB.push_back (make_tuple (selectname, Union (table, &selectQuery)));
                            cout << "Selection Successful.\n";
                            Union (table, &selectQuery)->print ();
                        }

                        else if (query == 4) { // Rename Attribute
                            string s1, s2;
                            cout << "Enter Old attribute name: "; cin >> s1;
                            cout << "Enter new attribute name: "; cin >> s2;
                            table = Difference (table, s1, s2);
                            cout << "Attribute renamed successfully.\n";
                        }
                        
                        else if (query == 5) {
                            table->print ();
                        }

                        else cout << "Enter a valid choice.\n";
                        
                    }

                }

                else cout << "Enter a valid choice.\n";
            }
        }

        else cout << "Enter Valid choice.\n";
    
    }

    // freeing allocated memory
    for (auto table : DB) delete get<1>(table);
    DB.clear ();

    return 0;
}