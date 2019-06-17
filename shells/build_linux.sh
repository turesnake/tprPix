# This script builds the sample for x64 Linux. 
# It assumes that both the dotnet CLI and g++ compiler are available on the path.

SCRIPTPATH=$(readlink -f "$0")

BASEDIR=$(dirname $SCRIPTPATH)
SRCDIR=${BASEDIR}/../csharp/
OUTDIR=${BASEDIR}/../build/bin/csharpLibs/

echo "------------"
echo ${BASEDIR}
echo ${SRCDIR}
echo ${OUTDIR}
echo "------------"


            #
            # 未完工，参见 osx 版 ...
            #


# Make output directory, if needed
if [ ! -d "${OUTDIR}" ]; then
    mkdir -p ${OUTDIR}
fi

# Build managed component
# echo Building Managed Library
# dotnet publish --self-contained -r linux-x64 ${SRCDIR}/ManagedLibrary/ManagedLibrary.csproj -o ${OUTDIR}

# Build native component
#g++ -o ${OUTDIR}/SampleHost -D LINUX ${SRCDIR}/SampleHost_origin.cpp -ldl






