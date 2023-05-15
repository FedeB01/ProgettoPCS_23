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
    for(unsigned int c = 0; c < maglia.NumeroCell2D; c++)
    {
      array<unsigned int, 3> lati = maglia.Cell2DLati[c];

      for(unsigned int e = 0; e < 3; e++)
      {

         const unsigned int inzio = maglia.Cell1DVertici[lati[e]][0];
         const unsigned int fine = maglia.Cell1DVertici[lati[e]][1];

         //cout<<origine<<"\t"<<end<<endl;

         auto findOrigin = find(maglia.Cell2DVertici[c].begin(), maglia.Cell2DVertici[c].end(), inzio);

         if(findOrigin == maglia.Cell2DVertici[c].end())
         {
           cerr << "Maglia sbagliata" << endl;
           return 2;
         }

         auto trovaFine = find(maglia.Cell2DVertici[c].begin(), maglia.Cell2DVertici[c].end(), fine);

         if(trovaFine == maglia.Cell2DVertici[c].end())
         {
           cerr << "Maglia sbagliata" << endl;
           return 3;
         }

      }
    }

*/


}

#endif // __TEST_EMPTY_H
