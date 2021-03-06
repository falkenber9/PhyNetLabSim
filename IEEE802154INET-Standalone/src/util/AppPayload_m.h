//
// Generated file, do not edit! Created by nedtool 4.6 from util/AppPayload.msg.
//

#ifndef _APPPAYLOAD_M_H_
#define _APPPAYLOAD_M_H_

#include <omnetpp.h>

// nedtool version check
#define MSGC_VERSION 0x0406
#if (MSGC_VERSION!=OMNETPP_VERSION)
#    error Version mismatch! Probably this file was generated by an earlier version of nedtool: 'make clean' should help.
#endif



/**
 * Class generated from <tt>util/AppPayload.msg:19</tt> by nedtool.
 * <pre>
 * //
 * // TODO generated message class
 * //
 * packet AppPayload
 * {
 *     long reqNr;
 *     int pageID;
 *     int hostIndex;
 * }
 * </pre>
 */
class AppPayload : public ::cPacket
{
  protected:
    long reqNr_var;
    int pageID_var;
    int hostIndex_var;

  private:
    void copy(const AppPayload& other);

  protected:
    // protected and unimplemented operator==(), to prevent accidental usage
    bool operator==(const AppPayload&);

  public:
    AppPayload(const char *name=NULL, int kind=0);
    AppPayload(const AppPayload& other);
    virtual ~AppPayload();
    AppPayload& operator=(const AppPayload& other);
    virtual AppPayload *dup() const {return new AppPayload(*this);}
    virtual void parsimPack(cCommBuffer *b);
    virtual void parsimUnpack(cCommBuffer *b);

    // field getter/setter methods
    virtual long getReqNr() const;
    virtual void setReqNr(long reqNr);
    virtual int getPageID() const;
    virtual void setPageID(int pageID);
    virtual int getHostIndex() const;
    virtual void setHostIndex(int hostIndex);
};

inline void doPacking(cCommBuffer *b, AppPayload& obj) {obj.parsimPack(b);}
inline void doUnpacking(cCommBuffer *b, AppPayload& obj) {obj.parsimUnpack(b);}


#endif // ifndef _APPPAYLOAD_M_H_

