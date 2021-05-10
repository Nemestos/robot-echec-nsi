#include "Arduino.h"
#include "Pool.h"
#include "Logger.h"
#include "KeyboardController.h"
#include "ArmController.h"
Pool &p_pool = Pool::getInst();
KeyboardController *p_keyboard = new KeyboardController();
Logger *p_logger = new Logger();
ArmController *p_arm = new ArmController();

void setup_arm()
{
    //allocation des differents composant du bras
    //ArmComponent *base = new ArmComponent("base", ArmsPin::ARM_BASE);
    ArmComponent *shoulder = new ArmComponent("shoulder", ArmsPin::ARM_SHOULDER);
    ArmComponent *elbow = new ArmComponent("elbow", ArmsPin::ARM_ELBOW);
    ArmComponent *wrist = new ArmComponent("wrist", ArmsPin::ARM_WRIST);
    ArmComponent *gripper = new ArmComponent("grippper", ArmsPin::ARM_GRIPPER);

    //allocation des differentes touches pour controller le bras(la clé et sa direction )
    Key *base_right = new Key("right", ArmDirection::RIGHT);
    Key *base_left = new Key("left", ArmDirection::LEFT);

    Key *shoulder_up = new Key("up", ArmDirection::UP);
    Key *shoulder_down = new Key("down", ArmDirection::DOWN);

    Key *elbow_up = new Key("z", ArmDirection::UP);
    Key *elbow_down = new Key("S", ArmDirection::DOWN);

    Key *wrist_up = new Key("p", ArmDirection::UP);
    Key *wrist_down = new Key("m", ArmDirection::DOWN);

    //transfert au controller qui ajoutes les components dans la liste
    //p_arm->addArmComponent(base);
    p_arm->addArmComponent(shoulder);
    p_arm->addArmComponent(elbow);
    p_arm->addArmComponent(wrist);
    p_arm->addArmComponent(gripper);

    //transfert des clés au controlleur
    p_arm->addKey(base_right);
    p_arm->addKey(base_left);

    p_arm->addKey(shoulder_up);
    p_arm->addKey(shoulder_down);

    p_arm->addKey(elbow_up);
    p_arm->addKey(elbow_down);

    p_arm->addKey(wrist_up);
    p_arm->addKey(wrist_down);
}
Servo x;
void setup()
{
    Serial.begin(9600);
    //on configure le tableaux de ressourses en convertissant chaque objets sous le type ressource
    Ressource *a_ressources[3] = {static_cast<Ressource *>(p_keyboard), static_cast<Ressource *>(p_logger), static_cast<Ressource *>(p_arm)};
    p_pool.add_ressources(a_ressources, 3);
    //setup_arm();
    x.attach(5);
}

void loop()
{
    x.write(80);
    p_keyboard->handlingCommand();
}