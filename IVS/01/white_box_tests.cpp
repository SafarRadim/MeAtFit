//======== Copyright (c) 2021, FIT VUT Brno, All rights reserved. ============//
//
// Purpose:     White Box - Tests suite
//
// $NoKeywords: $ivs_project_1 $white_box_code.cpp
// $Author:     RADIM SAFAR <xsafar27@stud.fit.vutbr.cz>
// $Date:       $2022-03-10
//============================================================================//
/**
 * @file white_box_tests.cpp
 * @author RADIM SAFAR
 * 
 * @brief Implementace testu prace s maticemi.
 */

#include "gtest/gtest.h"
#include "white_box_code.h"

//============================================================================//
// ** ZDE DOPLNTE TESTY **
//
// Zde doplnte testy operaci nad maticemi. Cilem testovani je:
// 1. Dosahnout maximalniho pokryti kodu (white_box_code.cpp) testy.
// 2. Overit spravne chovani operaci nad maticemi v zavislosti na rozmerech 
//    matic.
//============================================================================//

class SingleMatrixOperations : public ::testing::Test
{
    protected:
        Matrix SMatrix;
        Matrix LMatrix = Matrix(2, 2);
};

class MultipleMatrixOperations : public ::testing::Test
{
    protected:
        Matrix SMatrix_A;
        Matrix SMatrix_B;

        Matrix LMatrix_A = Matrix(3, 3);
        Matrix LMatrix_B = Matrix(3, 3);

        virtual void SetUp(){
            LMatrix_A.set({
                {0, 1, 0},
                {0, 0, 1},
                {1, 0, 0}
            });

            LMatrix_B.set({
                {0, 0, 1},
                {1, 0, 0},
                {0, 1, 0}
            });
        }
};

class ComplexMatrixOperations : public ::testing::Test
{
    protected:

        Matrix LMatrix = Matrix(3, 3);

        Matrix TMatrix = Matrix(2, 3);
        Matrix TMatrix_S = Matrix(3, 3);

        Matrix SMatrix = Matrix(2, 2);

        Matrix XSMatrix;
        Matrix XLMatrix = Matrix(5, 5);

        virtual void SetUp(){
            LMatrix.set({
                {4, 3, -3},
                {1, 1, -1},
                {2, 2, -1}
            });

            TMatrix_S.set({
                {1, 2, 3},
                {4, 5, 6},
                {7, 8, 9}
            });

            SMatrix.set({
                {4, 7},
                {2, 6}
            });

            XSMatrix.set(0, 0, 1);

            XLMatrix.set({
                {1, 0, 0, 0, 0},
                {0, 1, 0, 0, 0},
                {0, 0, 1, 0, 0},
                {0, 0, 0, 1, 0},
                {0, 0, 0, 0, 1}
            });
        }   
};



// Create matrix
// if R==0||C==0 then exception

TEST(MatrixCreate, OneByOneMatrix)
{
    Matrix matrix;
    EXPECT_EQ(matrix.get(0, 0), 0);
};

TEST(MatrixCreate, ThreeByThreeMatrix)
{
    Matrix matrix = Matrix(3, 3);
    EXPECT_EQ(matrix.get(2, 2), 0);
};

TEST(MatrixCreate, InvalidRMatrix)
{
    EXPECT_THROW(Matrix matrix = Matrix(0, 2), std::runtime_error);
};

TEST(MatrixCreate, InvalidCMatrix)
{
    EXPECT_THROW(Matrix matrix = Matrix(2, 0), std::runtime_error);
};

// Set value
// true if correct, false if failed

TEST_F(SingleMatrixOperations, SetProper)
{
    EXPECT_TRUE (SMatrix.set(0, 0, 5));
    EXPECT_EQ (SMatrix.get(0, 0), 5);
};

TEST_F(SingleMatrixOperations, SetImproper)
{
    EXPECT_FALSE (SMatrix.set(1, 1, 5));
};


// Set values
// true if correct, false if failed
TEST_F(SingleMatrixOperations, SetValsProper)
{
    std::vector<std::vector<double>> r = {
        {0, 1},
        {2, 3}
    };
    EXPECT_TRUE (LMatrix.set(r));
};

TEST_F(SingleMatrixOperations, SetValsImproper_C)
{
    std::vector<std::vector<double>> r = {
        {0, 1},
        {2, 3, 4}
    };
    EXPECT_FALSE (LMatrix.set(r));
};

TEST_F(SingleMatrixOperations, SetValsImproper_V)
{
    std::vector<std::vector<double>> r = {
        {0, 1},
        {2, 3},
        {4, 5}
    };
    EXPECT_FALSE(LMatrix.set(r));
}

// Get value
// Throws exception if coords out of matrix

TEST_F(SingleMatrixOperations, GetProper)
{
    EXPECT_EQ (SMatrix.get(0, 0), 0);
};

TEST_F(SingleMatrixOperations, GetInproper)
{
    EXPECT_THROW (SMatrix.get(1, 1), std::runtime_error);
};

// Compare matrix
// Throws exception if matrixes not same size
TEST_F(MultipleMatrixOperations, CompareProperS)
{
    EXPECT_TRUE (SMatrix_A == SMatrix_B);
}

TEST_F(MultipleMatrixOperations, CompareProperL)
{
    EXPECT_FALSE (LMatrix_A == LMatrix_B);
}

TEST_F(MultipleMatrixOperations, CompareImproperSize)
{
    EXPECT_THROW (SMatrix_A == LMatrix_A, std::runtime_error);
}

// Add matrix
// Throws exception if matrixes not same size
TEST_F(MultipleMatrixOperations, AddProperS)
{
    Matrix AddMatrix = SMatrix_A + SMatrix_B;
    EXPECT_EQ (AddMatrix.get(0, 0), 0);
}

TEST_F(MultipleMatrixOperations, AddProperL)
{
    Matrix AddMatrix = LMatrix_A + LMatrix_B;
    
    for(int i = 0; i < 3; i++)
    {
        for(int j = 0; j < 3; j++)
        {
            EXPECT_EQ(
                AddMatrix.get(i, j), 
                LMatrix_A.get(i, j) + LMatrix_B.get(i, j));
        }
    }
}

TEST_F(MultipleMatrixOperations, AddImproperSize)
{
    EXPECT_THROW(Matrix AddMatrix = LMatrix_A + SMatrix_A, std::runtime_error);
}

// Matrix multiplication
// Throws exception if not possible
TEST_F(MultipleMatrixOperations, MultProperS_Mat)
{
    Matrix MultMatrix = SMatrix_A * SMatrix_B;
    EXPECT_EQ(MultMatrix.get(0, 0), 0);
}

TEST_F(MultipleMatrixOperations, MultProperL_Mat)
{
    Matrix MultMatrix = LMatrix_A * LMatrix_B;

    Matrix OutMatrix = Matrix(3, 3);
    OutMatrix.set({
        {1, 0, 0},
        {0, 1, 0},
        {0, 0, 1}
    });

    EXPECT_EQ(MultMatrix, OutMatrix);
}

TEST_F(MultipleMatrixOperations, MultImproperSize_Mat)
{
    EXPECT_THROW(
        Matrix MultMatrix = LMatrix_A * SMatrix_A, 
        std::runtime_error
    );
}

// Scalar multiplication
TEST_F(MultipleMatrixOperations, MultProper_Scal)
{
    Matrix MultMatrix = LMatrix_A * 5;

    for(int i = 0; i < 3; i++)
    {
        for(int j = 0; j < 3; j++)
        {
            EXPECT_EQ(MultMatrix.get(i, j), LMatrix_A.get(i, j) * 5);
        }
    }
}

// Solve matrix
// Throws exception if unsolvable
TEST_F(ComplexMatrixOperations, SolveProperL)
{
    std::vector<double> in = {0, 0, 1};
    std::vector<double> MatSolution = LMatrix.solveEquation(in);
    
    std::vector<double> Solution = {0, 1, 1};

    EXPECT_EQ(MatSolution, Solution);
}

TEST_F(ComplexMatrixOperations, SolveProperXS)
{
    std::vector<double> in = {1};
    std::vector<double> MatSolution = XSMatrix.solveEquation(in);

    std::vector<double> Solution = {1};

    EXPECT_EQ(MatSolution, Solution);
}

TEST_F(ComplexMatrixOperations, SolveProperXL)
{
    std::vector<double> in = {1, 2, 3, 4, 5};
    std::vector<double> MatSolution = XLMatrix.solveEquation(in);

    EXPECT_EQ(MatSolution, in);
}

TEST_F(ComplexMatrixOperations, SolveImproperInput)
{
    std::vector<double> in = {1, 2};
    EXPECT_THROW(
        LMatrix.solveEquation(in),
        std::runtime_error
    );
}

TEST_F(ComplexMatrixOperations, SolveImproperMatrixSize)
{
    std::vector<double> in = {1, 2, 3};
    EXPECT_THROW(
        TMatrix.solveEquation(in),
        std::runtime_error
    );
}

TEST_F(ComplexMatrixOperations, SolveImproperSingular)
{
    std::vector<double> in = {1, 2, 3};
    EXPECT_THROW(
        TMatrix_S.solveEquation(in),
        std::runtime_error
    );
}

// Transpose matrix
TEST_F(ComplexMatrixOperations, Transpose)
{
    Matrix Matrix_T = Matrix(3, 3);

    Matrix_T.set({
        {4, 1, 2},
        {3, 1, 2},
        {-3, -1, -1}
    }); 

    EXPECT_EQ(LMatrix.transpose(), Matrix_T);
}

// Inverse matrix
// Only for 2x2 or 3x3

TEST_F(ComplexMatrixOperations, InverseProper3x3)
{
    Matrix InvMatrix = Matrix(3, 3);
    InvMatrix.set({
        {1, -3, 0},
        {-1, 2, 1},
        {0, -2, 1}
    });

    EXPECT_EQ(LMatrix.inverse(), InvMatrix);
}

TEST_F(ComplexMatrixOperations, InverseProper2x2)
{
    Matrix InvMatrix = Matrix(2, 2);
    InvMatrix.set({
        {0.6, -0.7},
        {-0.2, 0.4}
    });
    EXPECT_EQ(SMatrix.inverse(), InvMatrix);
}

TEST_F(ComplexMatrixOperations, InverseImproperSize)
{
    EXPECT_THROW(TMatrix.inverse(), std::runtime_error);
}

TEST_F(ComplexMatrixOperations, InverseImproperSingular)
{
    EXPECT_THROW(TMatrix_S.inverse(), std::runtime_error);
}

/*** Konec souboru white_box_tests.cpp ***/
