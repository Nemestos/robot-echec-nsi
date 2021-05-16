//classe utilisée pour contenir un mouvement specifique d'un servo avec un delay specifique entre les differents ajout de positions
class Movement
{
private:
    int m_angle_final;
    ArmComponent *m_comp;
    int m_delay;
    int m_last_time;
    int m_dir;

public:
    Movement(ArmComponent *comp, int final, int delay) : m_comp(comp), m_angle_final(final), m_delay(delay)
    {
        //direction en fonction de la position courante(ajouter ou supprimer)
        if (m_angle_final > m_comp->get_curr())
        {
            m_dir = 1;
        }
        else
        {
            m_dir = -1;
        }
    }
    //retourne si on a finit ou non
    bool isFinish()
    {
        return m_comp->get_curr() == m_angle_final;
    }

    void update()
    {
        //si l'intervalle entre le temps courant (en ms) et l'ancien temps est bien superieure
        if (millis() - this->m_last_time > this->m_delay)
        {
            //le delay a ete passé et on ajoute de la force
            m_comp->adding_value(m_dir);
            //Serial.println(this->m_comp->get_curr());
            //on met a jour l'ancien temps
            this->m_last_time = millis();
        }
    }
};