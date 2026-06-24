#include <gtest/gtest.h>
#include <Unittests/unittests_common.hh>

namespace {

class OpenMeshVectorApply : public OpenMeshBase {

    protected:

        // This function is called before each test is run
        virtual void SetUp() {

            // Do some initial stuff with the member data here...
        }

        // This function is called after all tests are through
        virtual void TearDown() {

            // Do some final stuff with the member data here...
        }

    // Member already defined in OpenMeshBase
    //Mesh mesh_;

};

/*
 * ====================================================================
 * Define tests below
 * ====================================================================
 */

// apply function to vector to see if it works correctly
TEST_F(OpenMeshVectorApply, ApplyFunctionToVector) {
    OpenMesh::Vec3d vec(1.0, 2.0, 3.0);
    auto squared = [](double x) { return x * x; };
    OpenMesh::Vec3d result = vec.apply(squared);

    EXPECT_DOUBLE_EQ(1.0, result[0]);
    EXPECT_DOUBLE_EQ(4.0, result[1]);
    EXPECT_DOUBLE_EQ(9.0, result[2]);
}
}
