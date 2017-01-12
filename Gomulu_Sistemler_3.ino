#include <avr/io.h>//avr kütüphanesi
#include <avr/interrupt.h>//kesme kütüphanesi
volatile int sayac=0;//saniyeyi tutar
volatile int ldr=0;//ldr kontrol eder
int main(void)
{
  cli();//kestinler kapatılır
  DDRD=B00011000;//0-7 arasındaki pinlerin içindeki 3. ve 4. output olarak tanımlandı
  
  TCCR1B|=(1<<WGM12);//timer1 ctc modda 
  TCCR1B|=(1<<CS12);//clk değeri 1024 olarak ayarlanır
  TCCR1B|=(1<<CS10);
  OCR1A=31250;//her 2 saniyede bir ISR fonksiyonunu tetiklemesini ister
  TIMSK1|=(1<<OCIE1A);
  PORTD=B0000000;
  
  sei();//kesmeleri tekrardan başlatır
  while(1)
  {
    ldr = (PIND&(1<<PD2));//ldr ye bağlı pini okur
    if(ldr>0)
    {//eğer ldr den değer okunursa durur motor
      PORTD=B0000000;
    }else
    {//değer yoksa ldr de motor çalışır
      if(sayac==0)
      {//ilk iki saniye kesme olmadığı için haraketsiz kalır
        PORTD=B0000000;
      }
      if(sayac>=1 && sayac<3)
      {
        PORTD=B0001000;//3. pine elektrik verilir
      }
      if(sayac==3)
      {
        PORTD=B0000000;
      }
      if(sayac>=4)
      {//motor saat yönünün tersine döner
        PORTD=B0010000;//4. pine elektrik verir
      }
    }

  }
}
ISR(TIMER1_COMPA_vect)//her bekleme süresinde bir buraya girer
{
  sayac++;//2 saniye de bir girerek sayacı arttırır
  if(sayac==3)
  {//buraya 3. girişinde 6 saniyeyi dolduru 
    //2 saniye durur ve 4 saniye haraket eder.
    //bu döngü 6 saniyede bir kendini tekrar eder
  }
  if(sayac==6)
  {//sonraki 6. saniyeye kadar da motor saat yönünün tersine döner
  //ve sayac sıfırlanır
  //bu döngüde 6 saniyede bir kendini tekrar eder
    sayac=0;
  }
}

