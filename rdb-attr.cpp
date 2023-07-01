#include "rdb.h"

// Attribute functions

Attr::Attr () {}
Attr::~Attr () {}

bool Attr::operator!= (const Attr& right) const { return !(*this == right); }
bool Attr::operator<= (const Attr& right) const { return (*this == right) || (*this < right); }
bool Attr::operator> (const Attr& right) const { return !(*this <= right); }
bool Attr::operator>= (const Attr& right) const { return !(*this < right); }

// Integer Attribute functions

integerAttr::integerAttr (int n) : _value (n) {} // ctor
integerAttr::integerAttr (const integerAttr& n) : _value (n._value) {} // cctor
integerAttr& integerAttr::operator= (const integerAttr& n) { _value = n._value; return *this; } // cassign
integerAttr::~integerAttr () {} // dtor

bool integerAttr::operator== (const Attr& right) const {
    const integerAttr *ptr = dynamic_cast<const integerAttr *>(&right);
    if (!ptr) return false;
    else return (ptr->_value == _value);
}

bool integerAttr::operator< (const Attr& right) const {
    const integerAttr *ptr = dynamic_cast<const integerAttr *>(&right);
    if (!ptr) return false;
    else return (_value < ptr->_value);
}

int integerAttr::getValue () const { return _value; }
void integerAttr::setValue (int n) { _value = n; }
int integerAttr::getType () const { return 0; }

// Float Attribute functions

floatAttr::floatAttr (float n) : _value (n) {} // ctor
floatAttr::floatAttr (const floatAttr& n) : _value (n._value) {} // cctor
floatAttr& floatAttr::operator= (const floatAttr& n) { _value = n._value; return *this; } // cassign
floatAttr::~floatAttr () {} // dtor

bool floatAttr::operator== (const Attr& right) const {
    const floatAttr *ptr = dynamic_cast<const floatAttr *>(&right);
    if (!ptr) return false;
    else return (ptr->_value == _value);
}

bool floatAttr::operator< (const Attr& right) const {
    const floatAttr *ptr = dynamic_cast<const floatAttr *>(&right);
    if (!ptr) return false;
    else return (_value < ptr->_value);
}

float floatAttr::getValue () const { return _value; }
void floatAttr::setValue (float n) { _value = n; }
int floatAttr::getType () const { return 1; }

// String Attribute functions

stringAttr::stringAttr (string s) : _value (s) {} // ctor
stringAttr::stringAttr (const stringAttr& s) : _value (s.getValue ()) {} // cctor
stringAttr& stringAttr::operator= (const stringAttr& s) { _value = s.getValue (); return *this; } // cassign
stringAttr::~stringAttr () {} // dtor

bool stringAttr::operator== (const Attr& right) const {
    const stringAttr *ptr = dynamic_cast<const stringAttr *>(&right);
    if (!ptr) return false;
    else return (ptr->_value == _value);
}

bool stringAttr::operator< (const Attr& right) const {
    const stringAttr *ptr = dynamic_cast<const stringAttr *>(&right);
    if (!ptr) return false;
    else if (_value.compare(ptr->_value) < 0) return true;
    else return false;
}

string stringAttr::getValue () const { return _value; }
void stringAttr::setValue (string s) { _value = s; }
int stringAttr::getType () const { return 2; }