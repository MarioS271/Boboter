/**
 * @file buzzer_task.cpp
 *
 * @authors MarioS271
 * @copyright MIT License
*/

#include "buzzer_task.hpp"

#include <esp_timer.h>
#include "types/system_context.hpp"
#include "types/flex.hpp"
#include "types/note.hpp"
#include "types/sound.hpp"

namespace Boboter::Tasks::Buzzer {
    void task(void* params) {
        using namespace Internal;
        using Boboter::Types::SystemContext;
        using Boboter::Types::flex;
        
        SystemContext* ctx = static_cast<SystemContext*>(params);
        Boboter::Libs::Buzzer& buzzer = ctx->buzzer;
        flex& 
    }
}