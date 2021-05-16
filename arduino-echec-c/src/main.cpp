#include "Arduino.h"
#include "Pool.h"
#include "Logger.h"
#include "KeyboardController.h"
#include "ArmController.h"
#include "MovementsController.h"

Pool &p_pool = Pool::getInst();
KeyboardController *p_keyboard = new KeyboardController();
Logger *p_logger = new Logger();
ArmController *p_arm = new ArmController();
MovementsController *p_mov = new MovementsController();

void setup_arm()
{
    //allocation des differents composant du bras
    ArmComponent *base = new ArmComponent("base", ArmsPin::ARM_BASE);
    // ArmComponent *shoulder = new ArmComponent("shoulder", ArmsPin::ARM_SHOULDER);
    // ArmComponent *elbow = new ArmComponent("elbow", ArmsPin::ARM_ELBOW);
    // ArmComponent *wrist = new ArmComponent("wrist", ArmsPin::ARM_WRIST);
    // ArmComponent *gripper = new ArmComponent("grippper", ArmsPin::ARM_GRIPPER);

    //setup des movements par default:
    p_mov->request_mov(base, 150, DELAY_MOVEMENT);
    // p_mov->request_mov(base, 0, DELAY_MOVEMENT);
    //p_mov->request_mov(base, 10, DELAY_MOVEMENT);

    //request_mov(base, 0, DELAY_MOVEMENT);
    // request_mov(shoulder, 150, DELAY_MOVEMENT);
    // request_mov(elbow, 150, DELAY_MOVEMENT);
    // request_mov(wrist, 150, DELAY_MOVEMENT);
    // request_mov(gripper, 150, DELAY_MOVEMENT);

    //allocation des differentes touches pour controller le bras(la clé et sa direction )
    Key *base_right = new Key("right", ArmDirection::RIGHT, base);
    Key *base_left = new Key("left", ArmDirection::LEFT, base);

    // Key *shoulder_up = new Key("up", ArmDirection::UP);
    // Key *shoulder_down = new Key("down", ArmDirection::DOWN);

    // Key *elbow_up = new Key("z", ArmDirection::UP);
    // Key *elbow_down = new Key("s", ArmDirection::DOWN);

    // Key *wrist_up = new Key("p", ArmDirection::UP);
    // Key *wrist_down = new Key("m", ArmDirection::DOWN);

    //transfert au controller qui ajoutes les components dans la liste
    p_arm->addArmComponent(base);
    // p_arm->addArmComponent(shoulder);
    // p_arm->addArmComponent(elbow);
    // p_arm->addArmComponent(wrist);
    // p_arm->addArmComponent(gripper);

    //transfert des clés au controlleur
    p_arm->addKey(base_right);
    p_arm->addKey(base_left);

    // p_arm->addKey(shoulder_up);
    // p_arm->addKey(shoulder_down);

    // p_arm->addKey(elbow_up);
    // p_arm->addKey(elbow_down);

    // p_arm->addKey(wrist_up);
    // p_arm->addKey(wrist_down);
}

void update_mov_queue()
{
    //on recupere l'element en tete de file
    Movement *first = p_mov->get_first_movement();
    first->update();       //on met a jour la position
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
    Ressource *a_ressources[4] = {static_cast<Ressource *>(p_keyboard), static_cast<Ressource *>(p_logger), static_cast<Ressource *>(p_arm), static_cast<Ressource *>(p_mov)};
    p_pool.add_ressources(a_ressources, 4);
    setup_arm();
}

void loop()
{
    p_keyboard->handlingCommand();
    //si il a des commandes a effectuer
    if (p_mov->movements_count() > 0)
    {
        update_mov_queue();
    }
}
