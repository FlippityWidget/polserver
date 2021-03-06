#ifndef UO_TOOL_MAIN_H
#define UO_TOOL_MAIN_H

#include "../clib/Program/ProgramMain.h"

#include <string>

namespace Pol
{
namespace UoTool
{
class UoToolMain : public Pol::Clib::ProgramMain
{
public:
  UoToolMain();
  virtual ~UoToolMain();

protected:
  virtual int main();

private:
  virtual void showHelp();
  int uotool();
};
}
}  // namespaces

#endif  // UO_TOOL_MAIN_H
