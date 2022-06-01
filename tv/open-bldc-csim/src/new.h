
/*
[1, 0, 0]
[1, 1, 0]
[0, 1, 0]
[0, 1, 1]
[0, 0, 1]
[1, 0, 1]


[1, 0, 0]
[1, 1, 0]
[0, 1, 0]
[0, 1, 1]
[0, 0, 1]
[1, 0, 1]
*/

enum SwitchState
{
    s1_0_0 = 0,
    s1_1_0,
    s0_1_0,
    s0_1_1,
    s0_0_1,
    s1_0_1
};

struct CommandVector
{
    uint8_t a;
    uint8_t b;
    uint8_t c;
}


SwitchState getInbetweenState(const SwitchState current_state, const SwitchState next_state)
{
    switch(current_state)
    {
        case s1_0_0:
            if(next_state == s1_0_1)
            {
                return s0_0_0;
            }
            else if(next_state == s1_1_0)
            {
                return s0_0_0;
            }
            else
            {
                // custom_exit(-1, "invalid next state")
                return s0_0_0;
            }
            break;
        case s1_1_0:
            if(next_state == s1_0_0)
            {
                return s0_0_0;
            }
            else if(next_state == s0_1_0)
            {
                return s0_0_0;
            }
            else
            {
                // custom_exit(-1, "invalid next state")
                return s0_0_0;
            }
            break;
        case s0_1_0:
            if(next_state == s1_1_0)
            {
                return s0_0_0;
            }
            else if(next_state == s0_1_1)
            {
                return s0_0_0;
            }
            else
            {
                // custom_exit(-1, "invalid next state")
                return s0_0_0;
            }
            break;
        case s0_1_1:
            
            break;
        case s0_0_1:
            
            break;
        case s1_0_1:
            
            break;
        default:
            return s1_0_1;

    }
}

void switchStateToSwitches(const SwitchState ss, Switches* switches)
{
    switch(ss)
    {
        case s1_0_0:
            switches->a = 1;
            switches->b = 0;
            switches->c = 0;
            break;
        case s1_1_0:
            switches->a = 1;
            switches->b = 0;
            switches->c = 0;
            break;
        case s0_1_0:
            switches->a = 0;
            switches->b = 1;
            switches->c = 0;
            break;
        case s0_1_1:
            switches->a = 0;
            switches->b = 1;
            switches->c = 1;
            break;
        case s0_0_1:
            switches->a = 0;
            switches->b = 0;
            switches->c = ;1
            break;
        case s1_0_1:
            switches->a = 1;
            switches->b = 0;
            switches->c = 1;
            break;
        default:
            switches->a = 0;
            switches->b = 0;
            switches->c = 0;

    }
}

void function()
{

}