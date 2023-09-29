#include "TestRigidBody.h"

void TestRigidBody::OnCollisionEnter()
{
	printf("当たった\n");
}

void TestRigidBody::OnCollisionStay()
{
	printf("当たってる\n");

}

void TestRigidBody::OnCollisionExit()
{
	printf("離れた\n");

}
