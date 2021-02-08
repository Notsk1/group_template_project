#include "creategame.hh"
#include "city.hh"

namespace Interface
{
    std::shared_ptr<ICity> createGame(){
    std::shared_ptr<ICity> city = std::make_shared<StudentSide::City>();
    return city;
    }
}
