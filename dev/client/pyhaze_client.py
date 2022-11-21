#!/usr/bin/env python

import os
import sys
import argparse
import numpy as np
import pyhaze
import nibabel.freesurfer.io as fsio
import warnings

def pyhazecl():

     # Parse command line arguments
    example_text = '''
    Run with '--help' to get help.
 '''

    parser = argparse.ArgumentParser(prog="pyhaze_client",
                                     description="Smooth per-vertex data on triangular meshes.",
                                     epilog=example_text,
                                     formatter_class=argparse.RawDescriptionHelpFormatter
                                     )
    parser.add_argument("pvdfile", help="str, the file containg the input per-vertex data.")
    parser.add_argument("meshfile", help="str, the mesh file for the input data.")
    parser.add_argument("-o", "--outputfile", help="str, output file name for smoothed per-vertex data. Defaults to 'smoothed.pvd'", default="smoothed.pvd")
    parser.add_argument("-i", "--numiter", help="int, number of smoothing iterations. Defaults to 5.", default="5")
    parser.add_argument("-e", "--via-edgelist", help="Compute mesh adjacency matrix via edge list instead of adjacency matrix. Only relevant if igl is not installed.", action="store_true")
    parser.add_argument("-n", "--no-nan", help="Disable support for NAN values in input data for small speed boost.", action="store_true")
    parser.add_argument("-w", "--no-warn-igl", help="Disable warning message in case of missing 'igl' package.", action="store_true")
    parser.add_argument("-s", "--silent", help="Disable printing of messages.", action="store_true")
    args = parser.parse_args()

    pvdfile = args.pvdfile
    meshfile = args.meshfile
    outfile = args.outputfile
    num_iter = int(args.numiter)
    with_nan = not args.no_nan
    via_matrix = not args.via_edgelist
    warn_igl = not args.no_warn_igl
    silent = args.silent

    if not silent:
        print(f"Smoothing pvd data from file '{pvdfile}' on mesh '{meshfile}' with {num_iter} iterations, with_nan={with_nan}.")

    if not os.path.isfile(pvdfile):
        raise ValueError(f"Cannot read input per-vertex data file '{pvdfile}'.")

    if not os.path.isfile(meshfile):
        raise ValueError(f"Cannot read input mesh file '{meshfile}'.")

    if meshfile == pvdfile:
        raise ValueError(f"The files passed as mesh file '{meshfile}' and per-vertex data file '{pvdfile}' cannot be identical.")


    vertices, faces = fsio.read_geometry(meshfile)
    pvd_data = fsio.read_morph_data(pvdfile)

    assert vertices.shape[0] == np.size(pvd_data), f"Length of input per-vertex data {np.size(pvd_data)} does not match number of mesh vertices {vertices.shape[0]}."

    if not silent:
        print(f" - Read mesh with {vertices.shape[0]} vertices and suitable per-vertex data.")

    has_igl = False
    try:
        import igl
        has_igl = True
    except Exception as ex:
        if warn_igl:
            warnings.warn(f"Python package 'igl' is not installed, falling back to slower computation of mesh adjacency list. Consider installing 'igl' via conda. : {str(ex)}")

    if has_igl:
        faces_igl = np.array(faces).reshape(-1, 3).astype(np.int64)
        mesh_adj = igl.adjacency_list(faces_igl)
        res = pyhaze.smooth_pvd_adj(mesh_adj, pvd_data.tolist(), num_iter=num_iter, with_nan=with_nan)
    else:
        res = pyhaze.smooth_pvd(vertices, faces, pvd_data.tolist(), num_iter=num_iter, via_matrix=via_matrix, with_nan=with_nan)

    assert np.size(res) == np.size(pvd_data), f"Length of smoothed per-vertex data {np.size(res)} does not match number of mesh vertices {vertices.shape[0]}."

    fsio.write_morph_data(outfile, res)

    if not silent:
        print(f" - Smoothed per-vertex data for {vertices.shape[0]} vertices written to output file '{outfile}'.")

if __name__ == "__main__":
    pyhazecl()