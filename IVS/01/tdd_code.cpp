//======== Copyright (c) 2021, FIT VUT Brno, All rights reserved. ============//
//
// Purpose:     Test Driven Development - priority queue code
//
// $NoKeywords: $ivs_project_1 $tdd_code.cpp
// $Author:     RADIM SAFAR <xsafar27@stud.fit.vutbr.cz>
// $Date:       $2022-03-10
//============================================================================//
/**
 * @file tdd_code.cpp
 * @author RADIM SAFAR
 * 
 * @brief Implementace metod tridy prioritni fronty.
 */

#include <stdlib.h>
#include <stdio.h>

#include "tdd_code.h"

//============================================================================//
// ** ZDE DOPLNTE IMPLEMENTACI **
//
// Zde doplnte implementaci verejneho rozhrani prioritni fronty (Priority Queue)
// 1. Verejne rozhrani fronty specifikovane v: tdd_code.h (sekce "public:")
//    - Konstruktor (PriorityQueue()), Destruktor (~PriorityQueue())
//    - Metody Insert/Remove/Find/GetHead ...
//    - Pripadne vase metody definovane v tdd_code.h (sekce "protected:")
//
// Cilem je dosahnout plne funkcni implementace prioritni fronty implementovane
// pomoci tzv. "singly linked list", ktera bude splnovat dodane testy
// (tdd_tests.cpp).
//============================================================================//

PriorityQueue::PriorityQueue()
{
    m_pHead = NULL;
}

PriorityQueue::~PriorityQueue()
{
    Element_t *currentPtr = GetHead();
    Element_t *secondPtr;
    
    while (currentPtr != NULL){
        secondPtr = currentPtr->pNext;
        delete(currentPtr);
        currentPtr = secondPtr;
    }
}

void PriorityQueue::Insert(int value)
{
    Element_t *currentPtr = GetHead();

    // Empty queue
    if(currentPtr == NULL){
        m_pHead = new Element_t();
        m_pHead->pNext = NULL;
        m_pHead->value = value;
        return;
    }

    // Value < Head Value
    if(currentPtr->value < value){
        Element_t *newElement = new Element_t();
        newElement->pNext = GetHead();
        newElement->value = value;
        m_pHead = newElement;
        return;
    }

    // Insert in middle
    Element_t *secondPtr = currentPtr;
    currentPtr = currentPtr->pNext;

    while(currentPtr != NULL){
        if(currentPtr->value < value){
            Element_t *newElement = new Element_t();
            newElement->pNext = currentPtr;
            newElement->value = value;

            secondPtr->pNext = newElement;
            return;
        }
        secondPtr = currentPtr;
        currentPtr = currentPtr->pNext;
    }

    // Insert as tail
    Element_t *newElement = new Element_t();
    newElement->pNext = NULL;
    newElement->value = value;
    secondPtr->pNext = newElement;
    return;
}

bool PriorityQueue::Remove(int value)
{
    Element_t *currentPtr = GetHead();
    Element_t *secondaryPtr = nullptr;

    while(currentPtr != NULL){
        if(currentPtr->value == value){
            if(secondaryPtr == nullptr){
                m_pHead = currentPtr->pNext;
                delete(currentPtr);
                return true;
            }

            secondaryPtr->pNext = currentPtr->pNext;
            delete(currentPtr);
            return true;
        }

        secondaryPtr = currentPtr;
        currentPtr = currentPtr->pNext;
    }

    return false;
}

PriorityQueue::Element_t *PriorityQueue::Find(int value)
{
    Element_t *currentPtr = GetHead();

    while(currentPtr != NULL){
        if(currentPtr->value == value){
            return currentPtr;
        }
        currentPtr = currentPtr->pNext;
    }

    return NULL;
}

size_t PriorityQueue::Length()
{
    int count = 0;
    Element_t *currentPtr = GetHead();

    while(currentPtr != NULL){
        count++;
        currentPtr = currentPtr->pNext;
    }

	return count;
}

PriorityQueue::Element_t *PriorityQueue::GetHead()
{
    if (m_pHead != NULL){
        return m_pHead;
    }
    return NULL;
}

/*** Konec souboru tdd_code.cpp ***/
