#include "Arduino.h"
#include "Pool.h"
#include "ArmController.h"
#include "MovementsController.h"
#include "Shortcuts.h"
Pool &p_pool = Pool::getInst();
ArmController *p_arm = new ArmController();
MovementsController *p_mov = new MovementsController();

//joystick
int joy_x = 1;
int joy_y = 2;
int joyval = 0;

//potentiometre
int pot_dt = 2;
int pot_clk = 3;
int pot_sw = 4;
int potval = 0;
int currCLK;
int prevCLK;

bool direct = true;

void setup_arm()
{
    //allocation des differents composant du bras
    ArmComponent *base = new ArmComponent("base", ArmsPin::ARM_BASE);
    // ArmComponent *shoulder = new ArmComponent("shoulder", ArmsPin::ARM_SHOULDER);
    // ArmComponent *elbow = new ArmComponent("elbow", ArmsPin::ARM_ELBOW);
    // ArmComponent *wrist = new ArmComponent("wrist", ArmsPin::ARM_WRIST);
    // ArmComponent *gripper = new ArmComponent("grippper", ArmsPin::ARM_GRIPPER);

    //setup des movements par default:
    //p_mov->request_mov(base, 0, DELAY_MOVEMENT);
    //p_mov->request_mov(base, 10, DELAY_MOVEMENT);
    //p_mov->request_mov(base, 150, DELAY_MOVEMENT);

    //request_mov(base, 0, DELAY_MOVEMENT);
    // request_mov(shoulder, 150, DELAY_MOVEMENT);
    // request_mov(elbow, 150, DELAY_MOVEMENT);
    // request_mov(wrist, 150, DELAY_MOVEMENT);
    // request_mov(gripper, 150, DELAY_MOVEMENT);

    //transfert au controller qui ajoutes les components dans la liste
    p_arm->addArmComponent(base);
    // p_arm->addArmComponent(shoulder);
    // p_arm->addArmComponent(elbow);
    // p_arm->addArmComponent(wrist);
    // p_arm->addArmComponent(gripper);
}

void setup_pot()
{
    pinMode(pot_clk, INPUT);
    pinMode(pot_dt, INPUT);
    prevCLK = digitalRead(pot_clk);
}

void update_pot()
{
    currCLK = digitalRead(pot_clk);
    //si l'etat courant different etat precedent, on a changé qq chose
    if (currCLK != prevCLK)
    {
        if (digitalRead(pot_dt) != currCLK)
        {
            potval--;
        }
        else
        {
            potval++;
        }
        potval = Utils::clamp<int>(potval, 0, 180);
        Serial.println(potval);
        p_arm->getComponent("base")->change_rotation(potval);
        prevCLK = currCLK;
    }
}

void update_joy()
{
    joyval = analogRead(joy_y);
    joyval = map(joyval, 0, 1023, 0, 180);
    Serial.println(joyval);
    p_arm->getComponent("base")->change_rotation(joyval);
}

void update_mov_queue()
{
    //on recupere l'element en tete de file
    Movement *first = p_mov->get_first_movement();
    first->update(); //on met a jour la position
    Serial.println(first->get_curr_rot());
    if (first->isFinish()) //si il a atteind la position
    {
        //on le supprime pour laisser la place au suivant
        p_mov->delete_first_movement();
    }
}
void setup()
{

    Serial.begin(9600);
    //on configure le tableaux de ressourses en convertissant chaque objets sous le type ressource
    Ressource *a_ressources[2] = {static_cast<Ressource *>(p_arm), static_cast<Ressource *>(p_mov)};
    p_pool.add_ressources(a_ressources, 2);
    setup_arm();
    setup_pot();
}

void loop()
{
    if (direct)
    {
        update_pot();
    }
    //si il a des commandes a effectuer
    else if (p_mov->movements_count() > 0)
    {
        update_mov_queue();
    }
}
