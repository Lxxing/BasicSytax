// -*- C++ -*-
// $Id$

/**
 * Code generated by the The ACE ORB (TAO) IDL Compiler v2.2.0
 * TAO and the TAO IDL Compiler have been developed by:
 *       Center for Distributed Object Computing
 *       Washington University
 *       St. Louis, MO
 *       USA
 *       http://www.cs.wustl.edu/~schmidt/doc-center.html
 * and
 *       Distributed Object Computing Laboratory
 *       University of California at Irvine
 *       Irvine, CA
 *       USA
 * and
 *       Institute for Software Integrated Systems
 *       Vanderbilt University
 *       Nashville, TN
 *       USA
 *       http://www.isis.vanderbilt.edu/
 *
 * Information about TAO is available at:
 *     http://www.cs.wustl.edu/~schmidt/TAO.html
 **/

// TAO_IDL - Generated from
// .\be\be_codegen.cpp:1673

#include "hello_Impl.h"

// Implementation skeleton constructor
Test_Hello_Impl::Test_Hello_Impl (void)
{
}

// Implementation skeleton destructor
Test_Hello_Impl::~Test_Hello_Impl (void)
{
}

char * Test_Hello_Impl::get_hello (
  void)
{
  // Add your implementation here
	return CORBA::string_dup ("Hello there!");
	///return "Hello there!";
}
