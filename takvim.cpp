#include "takvim.h"

Takvim::Takvim(QDate date, QPushButton *cagiranBtn)
{
    this->cagiranBtn = cagiranBtn;
    setSelectedDate(date);
    connect(this,SIGNAL(activated(QDate)),this, SLOT(secimYapildi(QDate)));
}

/**
 * @brief Takvim::secimYapildi : takvimde tarihe cift tiklanildigi ve enter a basildigi
 * zaman secimYapildi(QDate) slotu çalışıyor. secimYapildi(QDate)
 * takvim widget ini kapatiyor
 */
void Takvim::secimYapildi(QDate)
{
    close();
}

/**
 * @brief Takvim::closeEvent
 * takvim widget i kapandigi zaman closeEvent()
 * metodu calisiyor. closeEvent(QCloseEvent) takvimKapandi()
 * sinyalini veriyor ve takvim nesnesini siliyor
 */
void Takvim::closeEvent (QCloseEvent *)
{
    emit takvimKapandi(selectedDate(), cagiranBtn);
    delete this;
}

Takvim::~Takvim()
{

}
