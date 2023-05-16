#ifndef __TEST_EMPTY_H
#define __TEST_EMPTY_H

#include <gtest/gtest.h>
#include "classe_raffinamento.hpp"

using namespace testing;

TEST(TestEmpty, TestEmpty)
{

/*
  ProjectLibrary::Empty empty;
  ASSERT_NO_THROW(empty.Show());
*/


/*
    // Testaggio da implementare in futuro quando il programma sarà finito
    for(unsigned int c = 0; c < maglia.NumeroT; c++)
    {
      array<unsigned int, 3> lati = maglia.LatiT[c];

      for(unsigned int e = 0; e < 3; e++)
      {

         const unsigned int inzio = maglia.VerticiL[lati[e]][0];
         const unsigned int fine = maglia.VerticiL[lati[e]][1];

         //cout<<origine<<"\t"<<end<<endl;

         auto findOrigin = find(maglia.VerticiT[c].begin(), maglia.VerticiT[c].end(), inzio);

         if(findOrigin == maglia.VerticiT[c].end())
         {
           cerr << "Maglia sbagliata" << endl;
           return 2;
         }

         auto trovaFine = find(maglia.VerticiT[c].begin(), maglia.VerticiT[c].end(), fine);

         if(trovaFine == maglia.VerticiT[c].end())
         {
           cerr << "Maglia sbagliata" << endl;
           return 3;
         }

      }
    }

*/


}

#endif // __TEST_EMPTY_H
