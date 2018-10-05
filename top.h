#ifndef TOP_H
#define TOP_H

#include "AcceloratorIF.h"
#include "Gemmmodule.h"

SC_MODULE(Top)
{
  AcceloratorIF *acceloratorif;
  Gemmmodule   *gemmmodule;

  SC_CTOR(Top)
  {
    // Instantiate components
    acceloratorif = new AcceloratorIF("acceloratorif");
    gemmmodule   = new Gemmmodule   ("gemmmodule");

    // One initiator is bound directly to one target with no intervening bus

    // Bind initiator socket to target socket
    acceloratorif->socket.bind( gemmmodule->socket );
  }
};

#endif
