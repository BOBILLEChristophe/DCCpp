/**********************************************************************

Turnout.h renamed from Accessories.h
COPYRIGHT (c) 2013-2016 Gregg E. Berman

Part of DCC++ BASE STATION for the Arduino

**********************************************************************/

#ifndef Turnout_h
#define Turnout_h

#include "DCCpp.h"

#ifdef USE_TURNOUT
#include "Arduino.h"

struct TurnoutData {
  byte tStatus;
  byte subAddress;
  int id;
  int address;  
};

/** DCC++ BASE STATION can keep track of the direction of any turnout that is controlled
by a DCC stationary accessory decoder.  All turnouts, as well as any other DCC accessories
connected in this fashion, can always be operated using the DCC BASE STATION Accessory command:

\verbatim
<a ADDRESS SUBADDRESS ACTIVATE>
\endverbatim

However, this general command simply sends the appropriate DCC instruction packet to the main tracks
to operate connected accessories.  It does not store or retain any information regarding the current
status of that accessory.

To have this sketch store and retain the direction of DCC-connected turnouts, as well as automatically
invoke the required <a> command as needed, first define/edit/delete such turnouts using the following
variations of the "T" command:

\verbatim
<T ID ADDRESS SUBADDRESS>
\endverbatim
creates a new turnout ID, with specified ADDRESS and SUBADDRESS
if turnout ID already exists, it is updated with specificed ADDRESS and SUBADDRESS
returns: <O> if successful and <X> if unsuccessful (e.g. out of memory)

\verbatim
<T ID>
\endverbatim
deletes definition of turnout ID
returns: <O> if successful and <X> if unsuccessful (e.g. ID does not exist)

\verbatim
<T>
\endverbatim
lists all defined turnouts
returns: <H ID ADDRESS SUBADDRESS THROW> for each defined turnout or <X> if no turnouts defined

where

\verbatim
ID: the numeric ID (0-32767) of the turnout to control
ADDRESS:  the primary address of the decoder controlling this turnout (0-511)
SUBADDRESS: the subaddress of the decoder controlling this turnout (0-3)
\endverbatim

Once all turnouts have been properly defined, use the <E> command to store their definitions to EEPROM.
If you later make edits/additions/deletions to the turnout definitions, you must invoke the <E> command if you want those
new definitions updated in the EEPROM.  You can also clear everything stored in the EEPROM by invoking the <e> command.

To "throw" turnouts that have been defined use:

\verbatim
<T ID THROW>
\endverbatim
sets turnout ID to either the "thrown" or "unthrown" position
returns: <H ID THROW>, or <X> if turnout ID does not exist

where

\verbatim
ID: the numeric ID (0-32767) of the turnout to control
THROW: 0 (unthrown) or 1 (thrown)
\endverbatim

When controlled as such, the Arduino updates and stores the direction of each Turnout in EEPROM so
that it is retained even without power.  A list of the current directions of each Turnout in the form <H ID THROW> is generated
by this sketch whenever the <s> status command is invoked.  This provides an efficient way of initializing
the directions of any Turnouts being monitored or controlled by a separate interface or GUI program.
*/
struct Turnout{
  static Turnout *firstTurnout;
  int num;
  struct TurnoutData data;
  Turnout *nextTurnout;

  void activate(int s);

  static Turnout* get(int);
  static void remove(int);
  static Turnout *create(int, int, int, int=0);
  static void show(int=0);

#if defined(USE_TEXTCOMMAND)
  static void parse(char *c);
#if defined(USE_EEPROM)
  static void load();
  static void store();
#endif
#endif
}; // Turnout
  
#endif
#endif


