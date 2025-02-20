#!/usr/bin/env python
##
## @file    convertFbcToCobra.py
## @brief   Convert L3 with FBC to COBRA L2
## @author  Frank T. Bergmann
##
##
## <!--------------------------------------------------------------------------
## This sample program is distributed under a different license than the rest
## of libSBML.  This program uses the open-source MIT license, as follows:
##
## Copyright (c) 2013-2018 by the California Institute of Technology
## (California, USA), the European Bioinformatics Institute (EMBL-EBI, UK)
## and the University of Heidelberg (Germany), with support from the National
## Institutes of Health (USA) under grant R01GM070923.  All rights reserved.
##
## Permission is hereby granted, free of charge, to any person obtaining a
## copy of this software and associated documentation files (the "Software"),
## to deal in the Software without restriction, including without limitation
## the rights to use, copy, modify, merge, publish, distribute, sublicense,
## and/or sell copies of the Software, and to permit persons to whom the
## Software is furnished to do so, subject to the following conditions:
##
## The above copyright notice and this permission notice shall be included in
## all copies or substantial portions of the Software.
##
## THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
## IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
## FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
## THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
## LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
## FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
## DEALINGS IN THE SOFTWARE.
##
## Neither the name of the California Institute of Technology (Caltech), nor
## of the European Bioinformatics Institute (EMBL-EBI), nor of the University
## of Heidelberg, nor the names of any contributors, may be used to endorse
## or promote products derived from this software without specific prior
## written permission.
## ------------------------------------------------------------------------ -->

import sys
import os.path
import libsbml


def main(args):
    """usage: convertFbcToCobra.py input-filename output-filename
    """
    if len(args) != 3:
        print(main.__doc__)
        sys.exit(1)

    infile = args[1]
    outfile = args[2]

    if not os.path.exists(infile):
        print("[Error] %s : No such file." % infile)
        sys.exit(1)

    reader = libsbml.SBMLReader()
    writer = libsbml.SBMLWriter()
    sbmldoc = reader.readSBML(infile)

    if sbmldoc.getNumErrors() > 0:
        if sbmldoc.getError(0).getErrorId() == libsbml.XMLFileUnreadable:
            # Handle case of unreadable file here.
            sbmldoc.printErrors()
        elif sbmldoc.getError(0).getErrorId() == libsbml.XMLFileOperationError:
            # Handle case of other file error here.
            sbmldoc.printErrors()
        else:
            # Handle other error cases here.
            sbmldoc.printErrors()

            # sys.exit(1)

    # strip non-FBC plugins
    for p_ in range(sbmldoc.getNumPlugins()):
        if sbmldoc.getPlugin(p_).getPackageName() != 'fbc':
            props = libsbml.ConversionProperties()
            props.addOption("stripPackage", True, "Strip SBML Level 3 package constructs from the model")
            props.addOption("package", sbmldoc.getPlugin(p_).getPackageName(), "Name of the SBML Level 3 package to be stripped")
            if sbmldoc.convert(props) != libsbml.LIBSBML_OPERATION_SUCCESS:
                print("[Error] Failed to remove package: {}".format(sbmldoc.getPlugin(p_).getPackageName()))

    # convert to L2
    props = libsbml.ConversionProperties()
    props.addOption("convert fbc to cobra", True, "Convert FBC model to Cobra model")
    result = sbmldoc.convert(props)

    if result != libsbml.LIBSBML_OPERATION_SUCCESS:
        print("[Error] Conversion failed... (%d)" % result)
        sys.exit(1)

    writer.writeSBML(sbmldoc, outfile)
    print("[OK] converted file %s to %s" % (infile, outfile))


if __name__ == '__main__':
    main(sys.argv)
