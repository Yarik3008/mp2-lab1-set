// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле

#include "tbitfield.h"

// Fake variables used as placeholders in tests
static const int FAKE_INT = -1;
static TBitField FAKE_BITFIELD(1);

TBitField::TBitField(int len)
{
    if (len < 0) {
        throw (len);
    }
    BitLen = len;
    MemLen = (len - 1) / (sizeof(TELEM)*8 ) + 1;
    pMem = new TELEM[MemLen];
    for (int i = 0; i < MemLen; i++)
    {
        pMem[i] = 0;
    }
}

TBitField::TBitField(const TBitField &bf) // конструктор копирования
{
    BitLen = bf.BitLen;
    MemLen = bf.MemLen;
    pMem = new TELEM[MemLen];
    for (int i = 0; i < MemLen; i++)
    {
        pMem[i] = bf.pMem[i];
    }
}

TBitField::~TBitField()
{
    delete[] pMem;
}

int TBitField::GetMemIndex(const int n) const // индекс Мем для бита n
{
    if (n<0 || n>BitLen) { throw n; }
    return (n / (sizeof(TELEM) * 8));
}

TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n
{
    if ((n < 0) || (n >= BitLen)) { throw n; }
    else
    {
        return 1 << (n - 1) % (8 * sizeof(TELEM));
    }
    
}

// доступ к битам битового поля

int TBitField::GetLength(void) const // получить длину (к-во битов)
{

    return BitLen;
}

void TBitField::SetBit(const int n) // установить бит
{
    if (n < 0 || n >= BitLen) {
        throw n;
    }
    int index = GetMemIndex(n);
    TELEM Mask = GetMemMask(n);
    pMem[index] = pMem[index] | Mask;
}

void TBitField::ClrBit(const int n) // очистить бит
{
    if (n < 0 || n >= BitLen) {
        throw n;
    }
    pMem[GetMemIndex(n)] = pMem[GetMemIndex(n)] & ~GetMemMask(n); //первоначально - 01001100 очистить бит 4 - 01001100 & 111101111
}

int TBitField::GetBit(const int n) const // получить значение бита
{
    if (n < 0 || n >= BitLen) {
        throw n;
    }
    return (pMem[GetMemIndex(n)] & GetMemMask(n)); 
}

// битовые операции

TBitField& TBitField::operator=(const TBitField &bf) // присваивание
{

    if (&bf == this)
    {
        return *this;
    }

    if (MemLen != bf.MemLen)
    {
        delete[] pMem;
        pMem = new TELEM[MemLen];
        MemLen = bf.MemLen;
    }
    BitLen = bf.BitLen;
    for (int i = 0; i < MemLen; i++) {
        pMem[i] = bf.pMem[i];
    }
    return *this;
}

int TBitField::operator==(const TBitField &bf) const // сравнение
{
    if (BitLen != bf.BitLen) {
        return 0;  
    }
    for (int i = 0; i < MemLen; i++) {
        if (pMem[i] != bf.pMem[i]) {
            return 0;  
        }
    }
    return 1;
}

int TBitField::operator!=(const TBitField &bf) const // сравнение
{
    return !(*this == bf);
}

TBitField TBitField::operator|(const TBitField &bf) // операция "или"
{
    int l; 
    if (bf.BitLen > BitLen) {
        l = bf.BitLen; //5 = 00000101 7 = 0000111 
    }
    else {
        l = BitLen;
    }
    TBitField Field(l);

    for (int i = 0; i < bf.MemLen; i++) {
        Field.pMem[i] = pMem[i] | bf.pMem[i]; //00001101 00000010 = 00001111
    }
    return Field;
}

TBitField TBitField::operator&(const TBitField &bf) // операция "и"
{
    int l;
    if (bf.BitLen > BitLen) {
        l = bf.BitLen;
    }
    else {
        l = BitLen;
    }
    TBitField Field(l);
    for (int i = 0; i < bf.MemLen; i++)
    {
        Field.pMem[i] = bf.pMem[i];
    }
    for (int i = 0; i < bf.MemLen; i++) {
        Field.pMem[i] = Field.pMem[i] & pMem[i];
    }
    return Field;
}

TBitField TBitField::operator~(void) // отрицание 
{

    TBitField Field(*this);
    for (int i = 0; i < MemLen - 1; i++) { 
        Field.pMem[i] = ~Field.pMem[i]; 
    }
    if (MemLen > 0) {
        for (int i = (MemLen - 1) * sizeof(TELEM)* 8; i < Field.BitLen; i++) {
            if (Field.GetBit(i)) {
                Field.ClrBit(i);
            }
            else
                Field.SetBit(i);
        }
    }
    return Field;
}

// ввод/вывод

istream &operator>>(istream &istr, TBitField &bf) // ввод
{
    int i = 0;
    char ch;
    do { istr >> ch; } while (ch != ' ');
    while (true)
    {
        istr >> ch;
        if (ch == '0')
            bf.ClrBit(i++);
        else if (ch == '1')
            bf.SetBit(i++);
        else break;
    }
    return istr;
}

ostream &operator<<(ostream &ostr, const TBitField &bf) // вывод
{
    for (int i = 0; i < bf.BitLen; i++) {
        if (bf.GetBit(i))
            ostr << 1;
        else
            ostr << 0;
    }
    return ostr;
}
