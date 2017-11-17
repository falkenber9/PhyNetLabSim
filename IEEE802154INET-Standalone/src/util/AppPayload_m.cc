//
// Generated file, do not edit! Created by nedtool 4.6 from util/AppPayload.msg.
//

// Disable warnings about unused variables, empty switch stmts, etc:
#ifdef _MSC_VER
#  pragma warning(disable:4101)
#  pragma warning(disable:4065)
#endif

#include <iostream>
#include <sstream>
#include "AppPayload_m.h"

USING_NAMESPACE


// Another default rule (prevents compiler from choosing base class' doPacking())
template<typename T>
void doPacking(cCommBuffer *, T& t) {
    throw cRuntimeError("Parsim error: no doPacking() function for type %s or its base class (check .msg and _m.cc/h files!)",opp_typename(typeid(t)));
}

template<typename T>
void doUnpacking(cCommBuffer *, T& t) {
    throw cRuntimeError("Parsim error: no doUnpacking() function for type %s or its base class (check .msg and _m.cc/h files!)",opp_typename(typeid(t)));
}




// Template rule for outputting std::vector<T> types
template<typename T, typename A>
inline std::ostream& operator<<(std::ostream& out, const std::vector<T,A>& vec)
{
    out.put('{');
    for(typename std::vector<T,A>::const_iterator it = vec.begin(); it != vec.end(); ++it)
    {
        if (it != vec.begin()) {
            out.put(','); out.put(' ');
        }
        out << *it;
    }
    out.put('}');
    
    char buf[32];
    sprintf(buf, " (size=%u)", (unsigned int)vec.size());
    out.write(buf, strlen(buf));
    return out;
}

// Template rule which fires if a struct or class doesn't have operator<<
template<typename T>
inline std::ostream& operator<<(std::ostream& out,const T&) {return out;}

Register_Class(AppPayload);

AppPayload::AppPayload(const char *name, int kind) : ::cPacket(name,kind)
{
    this->reqNr_var = 0;
    this->pageID_var = 0;
    this->hostIndex_var = 0;
}

AppPayload::AppPayload(const AppPayload& other) : ::cPacket(other)
{
    copy(other);
}

AppPayload::~AppPayload()
{
}

AppPayload& AppPayload::operator=(const AppPayload& other)
{
    if (this==&other) return *this;
    ::cPacket::operator=(other);
    copy(other);
    return *this;
}

void AppPayload::copy(const AppPayload& other)
{
    this->reqNr_var = other.reqNr_var;
    this->pageID_var = other.pageID_var;
    this->hostIndex_var = other.hostIndex_var;
}

void AppPayload::parsimPack(cCommBuffer *b)
{
    ::cPacket::parsimPack(b);
    doPacking(b,this->reqNr_var);
    doPacking(b,this->pageID_var);
    doPacking(b,this->hostIndex_var);
}

void AppPayload::parsimUnpack(cCommBuffer *b)
{
    ::cPacket::parsimUnpack(b);
    doUnpacking(b,this->reqNr_var);
    doUnpacking(b,this->pageID_var);
    doUnpacking(b,this->hostIndex_var);
}

long AppPayload::getReqNr() const
{
    return reqNr_var;
}

void AppPayload::setReqNr(long reqNr)
{
    this->reqNr_var = reqNr;
}

int AppPayload::getPageID() const
{
    return pageID_var;
}

void AppPayload::setPageID(int pageID)
{
    this->pageID_var = pageID;
}

int AppPayload::getHostIndex() const
{
    return hostIndex_var;
}

void AppPayload::setHostIndex(int hostIndex)
{
    this->hostIndex_var = hostIndex;
}

class AppPayloadDescriptor : public cClassDescriptor
{
  public:
    AppPayloadDescriptor();
    virtual ~AppPayloadDescriptor();

    virtual bool doesSupport(cObject *obj) const;
    virtual const char *getProperty(const char *propertyname) const;
    virtual int getFieldCount(void *object) const;
    virtual const char *getFieldName(void *object, int field) const;
    virtual int findField(void *object, const char *fieldName) const;
    virtual unsigned int getFieldTypeFlags(void *object, int field) const;
    virtual const char *getFieldTypeString(void *object, int field) const;
    virtual const char *getFieldProperty(void *object, int field, const char *propertyname) const;
    virtual int getArraySize(void *object, int field) const;

    virtual std::string getFieldAsString(void *object, int field, int i) const;
    virtual bool setFieldAsString(void *object, int field, int i, const char *value) const;

    virtual const char *getFieldStructName(void *object, int field) const;
    virtual void *getFieldStructPointer(void *object, int field, int i) const;
};

Register_ClassDescriptor(AppPayloadDescriptor);

AppPayloadDescriptor::AppPayloadDescriptor() : cClassDescriptor("AppPayload", "cPacket")
{
}

AppPayloadDescriptor::~AppPayloadDescriptor()
{
}

bool AppPayloadDescriptor::doesSupport(cObject *obj) const
{
    return dynamic_cast<AppPayload *>(obj)!=NULL;
}

const char *AppPayloadDescriptor::getProperty(const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : NULL;
}

int AppPayloadDescriptor::getFieldCount(void *object) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 3+basedesc->getFieldCount(object) : 3;
}

unsigned int AppPayloadDescriptor::getFieldTypeFlags(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeFlags(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
    };
    return (field>=0 && field<3) ? fieldTypeFlags[field] : 0;
}

const char *AppPayloadDescriptor::getFieldName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldNames[] = {
        "reqNr",
        "pageID",
        "hostIndex",
    };
    return (field>=0 && field<3) ? fieldNames[field] : NULL;
}

int AppPayloadDescriptor::findField(void *object, const char *fieldName) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount(object) : 0;
    if (fieldName[0]=='r' && strcmp(fieldName, "reqNr")==0) return base+0;
    if (fieldName[0]=='p' && strcmp(fieldName, "pageID")==0) return base+1;
    if (fieldName[0]=='h' && strcmp(fieldName, "hostIndex")==0) return base+2;
    return basedesc ? basedesc->findField(object, fieldName) : -1;
}

const char *AppPayloadDescriptor::getFieldTypeString(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeString(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldTypeStrings[] = {
        "long",
        "int",
        "int",
    };
    return (field>=0 && field<3) ? fieldTypeStrings[field] : NULL;
}

const char *AppPayloadDescriptor::getFieldProperty(void *object, int field, const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldProperty(object, field, propertyname);
        field -= basedesc->getFieldCount(object);
    }
    switch (field) {
        default: return NULL;
    }
}

int AppPayloadDescriptor::getArraySize(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getArraySize(object, field);
        field -= basedesc->getFieldCount(object);
    }
    AppPayload *pp = (AppPayload *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

std::string AppPayloadDescriptor::getFieldAsString(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldAsString(object,field,i);
        field -= basedesc->getFieldCount(object);
    }
    AppPayload *pp = (AppPayload *)object; (void)pp;
    switch (field) {
        case 0: return long2string(pp->getReqNr());
        case 1: return long2string(pp->getPageID());
        case 2: return long2string(pp->getHostIndex());
        default: return "";
    }
}

bool AppPayloadDescriptor::setFieldAsString(void *object, int field, int i, const char *value) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->setFieldAsString(object,field,i,value);
        field -= basedesc->getFieldCount(object);
    }
    AppPayload *pp = (AppPayload *)object; (void)pp;
    switch (field) {
        case 0: pp->setReqNr(string2long(value)); return true;
        case 1: pp->setPageID(string2long(value)); return true;
        case 2: pp->setHostIndex(string2long(value)); return true;
        default: return false;
    }
}

const char *AppPayloadDescriptor::getFieldStructName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    switch (field) {
        default: return NULL;
    };
}

void *AppPayloadDescriptor::getFieldStructPointer(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructPointer(object, field, i);
        field -= basedesc->getFieldCount(object);
    }
    AppPayload *pp = (AppPayload *)object; (void)pp;
    switch (field) {
        default: return NULL;
    }
}


