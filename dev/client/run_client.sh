#!/bin/sh
#
# This runs the demo pyhaze client script `pyhaze_client.py`.
#
# Note: That script uses packages which are not dependencies of pyhaze,
#       so you will have to install them if you do not have them already.
#       To install everything in a fresh environment, run:
#
#           pip install numpy nibabel pyhaze
#
#       and, optional but very helpful for a speed boost:
#
#           conda install -y -c conda-forge igl
#
#

# Check whether we are in repo root, try to change into it if not.
if [ ! -d "tests" ]; then
    if [ -f "pyhaze_client.py" ]; then
      # User called this from the dir it is in, change 2 down.
      cd ../../
    else
        # No idea where they called this from.
        echo "ERROR: Please call this script from the repo root. Exiting."
        exit 1
    fi
fi

run_with_nonan="true"

surf_dir="./tests/test_data/tim_only/tim/surf"
surface="white"
num_iter="150"

for descriptor in thickness curv; do
    for hemi in lh rh; do
        mesh_file="${surf_dir}/${hemi}.${surface}"
        pvd_file="${surf_dir}/${hemi}.${descriptor}"
        output_file="${surf_dir}/${hemi}.smooth${num_iter}_${descriptor}"
        ./dev/client/pyhaze_client.py "${pvd_file}" "${mesh_file}" -o "${output_file}" -i ${num_iter}

        if [ "${run_with_nonan}" = "true" ]; then
            output_file_nonan="${surf_dir}/${hemi}.nonan_smooth${num_iter}_${descriptor}"
            ./dev/client/pyhaze_client.py "${pvd_file}" "${mesh_file}" -o "${output_file_nonan}" -i ${num_iter} --no-nan
        fi

    done
done


# Hint: To visualize the results, the easiest is maybe R currently:
# $ R
# > install.packages("fsbrain")
# > fsbrain::vis.subject.morph.native("tests/test_data/tim_only", "tim", "thickness")
# > fsbrain::vis.subject.morph.native("tests/test_data/tim_only", "tim", "smooth150_thickness")
# > fsbrain::vis.subject.morph.native("tests/test_data/tim_only", "tim", "nonan_smooth150_thickness")


