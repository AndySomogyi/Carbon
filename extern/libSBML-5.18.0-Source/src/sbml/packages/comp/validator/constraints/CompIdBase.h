/**
 * @cond doxygenLibsbmlInternal
 *
 * @file    CompIdBase.h
 * @brief   Base class for Id constraints
 * @author  Ben Bornstein
 * 
 * <!--------------------------------------------------------------------------
 * This file is part of libSBML.  Please visit http://sbml.org for more
 * information about SBML, and the latest version of libSBML.
 *
 * Copyright (C) 2019 jointly by the following organizations:
 *     1. California Institute of Technology, Pasadena, CA, USA
 *     2. University of Heidelberg, Heidelberg, Germany
 *
 * Copyright (C) 2013-2018 jointly by the following organizations:
 *     1. California Institute of Technology, Pasadena, CA, USA
 *     2. EMBL European Bioinformatics Institute (EMBL-EBI), Hinxton, UK
 *     3. University of Heidelberg, Heidelberg, Germany
 * 
 * Copyright 2011-2012 jointly by the following organizations:
 *     1. California Institute of Technology, Pasadena, CA, USA
 *     2. EMBL European Bioinformatics Institute (EMBL-EBI), Hinxton, UK
 *
 * This library is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation.  A copy of the license agreement is provided
 * in the file named "LICENSE.txt" included with this software distribution
 * and also available online as http://sbml.org/software/libsbml/license.html
 * ---------------------------------------------------------------------- -->*/

#ifndef CompIdBase_h
#define CompIdBase_h


#ifdef __cplusplus


#include <string>
#include <sbml/validator/VConstraint.h>
#include <sbml/packages/comp/common/CompExtensionTypes.h>

LIBSBML_CPP_NAMESPACE_BEGIN

class SBase;
class CompValidator;


/**
 * The CompIdBase Constraint is the base class for all SBML id constraints.  It
 * provides mechanisms for checking only certain subsets of ids within an
 * SBML model and tailoring the error messages logged.
 *
 * To customize:
 *
 *   1.  Override doCheck() to iterate over the SBML objects you are
 *       interested in and call checkId() for each.
 *
 *       checkId() does the work of extracting the unique identifier
 *       (whether it be an id or variable name) from the SBML object and
 *       then delegates to doCheckId().
 *
 *   2.  Override doCheckId() to perform the actual check.  If the check
 *       fails, call logFailure().
 *
 *   3.  Override getMessage() to log error messages.  GetMessage() should
 *       use getPreamble() and getFieldName() when constructing the error
 *       message.
 *
 *   4.  Override getPreamble() to customize the part of the actual error
 *       message that remains constant (e.g. the part that doesn't report
 *       line numbers, SBML object ids and typenames, etc).
 *
 *   5.  Override getFieldName() if you are checking a field that isn't
 *       called 'id', for instance, the 'variable' field of a Rule.
 *
 * Finally, if you need the type name of the SBML object that failed,
 * e.g. 'Compartment' or 'Species', when constructing an error message,
 * call getTypename().
 */
class CompIdBase: public TConstraint<Model>
{
public:

  /**
   * Creates a new CompIdBase with the given constraint id.
   */
  CompIdBase (unsigned int id, CompValidator& v);

  /**
   * Destroys this Constraint.
   */
  virtual ~CompIdBase ();


protected:


  /**
   * Called by check().  Override this method to define your own subset.
   */
  virtual void doCheck (const Model& m) = 0;

  /**
   * Checks that the id associated with the given object adheres to this
   * constraint.  If it does not, logFailure is called.
   */
  virtual void doCheckId (const std::string& id, const SBase& object) = 0;

  /**
   * Returns the fieldname to use when logging constraint violations.  Subclasses
   * are supposed to override this method if "id" is not appropriate.
   *
   * @return the string "id".
   */
  virtual const char* getFieldname ();

  /**
   * Returns the preamble to use when logging constraint violations.  
   * An empty string by default, any subclasses
   * are supposed to override this method if this is not appropriate.
   *
   * @return an empty string (the preamble to use when logging constraint violations).
   */
  virtual const char* getPreamble ();

  /**
   * Returns the error message to use when logging constraint violations.
   * This method must be overridden by subclasses.
   *
   * If at all possible please use getPreamble() and getFieldname() when
   * constructing error messages.  This will help to make your constraint
   * easily customizable.
   * 
   * @return the error message to use when logging constraint violations.
   */
  virtual const std::string
  getMessage (const std::string& id, const SBase& object) = 0;



  /* ------------------------------------------------------------ */
  /*  You should not need to override methods beyond this point.  */
  /* ------------------------------------------------------------ */


  /**
   * Checks that all ids for some given subset of the Model adhere to this
   * Constraint.  Override the doCheck() method to define your own subset.
   */
  virtual void check_ (const Model& m, const Model& object);

  /*
   * These convenience methods simply wrap (delegate to) doCheckId(const
   * std::string&, const SBase& object).  This is necessary because getId()
   * is not (yet) defined on SBase for SBML objects.
   *
   * For Rules and EventAssignments, it calls getVariable() instead.
   */

  void checkId (const Model&              x);
  void checkId (const Submodel& x);
  void checkId (const Port&     x);
  void checkId (const Deletion&        x);
  void checkId (const ReplacedElement&            x);
  void checkId (const ReplacedBy&          x);
  void checkId (const ModelDefinition&               x);
  void checkId (const ExternalModelDefinition&           x);
  void checkId (const FunctionDefinition& x);
  void checkId (const UnitDefinition&     x);
  void checkId (const Compartment&        x);
  void checkId (const Species&            x);
  void checkId (const Parameter&          x);
  void checkId (const Rule&               x);
  void checkId (const Reaction&           x);
  void checkId (const Event&              x);
  void checkId (const EventAssignment&    x);
  void checkId (const CompartmentType&    x);
  void checkId (const SpeciesType&        x);
  void checkId (const SpeciesReference&   x);
  void checkId (const ModifierSpeciesReference&              x);
  void checkId (const InitialAssignment&  x);
//  void checkId (const SBase&    x);

  /**
   * Returns a non-owning character pointer to the typename of the given SBase 
   * @p object, as constructed from its typecode and package.
   *
   * @return the typename of the given SBase object.
   */
  const char* getTypename (const SBase& object);

  /**
   * Logs a message that the given @p id (and its corresponding object) have
   * failed to satisfy this constraint.
   */
  void logIdConflict (const std::string& id, const SBase& object);
};

LIBSBML_CPP_NAMESPACE_END

#endif  /* __cplusplus */
#endif  /* CompIdBase_h */

/** @cond doxygenLibsbmlInternal */
