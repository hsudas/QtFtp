#include "takvim.h"

Takvim::Takvim(QDate date)
{
    setSelectedDate(date);
    connect(this,SIGNAL(activated(QDate)),this, SLOT(secimYapildi(QDate)));
}

/*
 * takvimde tarihe cift tiklanildigi ve enter a basildigi
 * zaman secimYapildi(QDate) slotu çalışıyor. secimYapildi(QDate)
 * takvim widget ini kapatiyor
 */
void Takvim::secimYapildi(QDate date)
{
    close();
}

/*
 * takvim widget i kapandigi zaman closeEvent(QCloseEvent)
 * metodu calisiyor. closeEvent(QCloseEvent) takvimKapandi(QDate)
 * sinyalini veriyor ve takvim nesnesini siliyor
 */
void Takvim::closeEvent (QCloseEvent *event)
{
    emit takvimKapandi(selectedDate());
    delete this;
}

Takvim::~Takvim()
{

}
