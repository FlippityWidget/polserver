#include "PolMain.h"

#include "pol.h"  //TODO: remove this ugly hack asap again

namespace Pol
{
namespace Clib
{
using namespace std;

///////////////////////////////////////////////////////////////////////////////

PolMain::PolMain() : ProgramMain()
{
}
PolMain::~PolMain()
{
}
///////////////////////////////////////////////////////////////////////////////
void PolMain::showHelp()
{
  // no help available
}

int PolMain::main()
{
  // TODO: merge the following with all the other xmain* functions
  return Pol::xmain_outer();
}
}
}  // namespaces

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

int main( int argc, char* argv[] )
{
  Pol::Clib::PolMain* polMain = new Pol::Clib::PolMain();
  polMain->start( argc, argv );
}
