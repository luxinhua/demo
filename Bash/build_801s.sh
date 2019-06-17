#!/bin/bash
#
# Description
#   Script to build adsp.
#
#   1.Variables could be used as default parameters.
#   2.All commands have log after excution, or with start / end pair.
#   3.Capture build log.
#
# Author
#   huanghlwk@sina.com
#
# Version
#   2017-09-04  huanghlwk   Build adsp only.
#
################################################################################

################################################################################
# 1. General Action
################################################################################

# 1.1 Default variables
SCRIPT_NAME=`basename $0`
SCRIPT_VERSION=0.1
USER_NAME=`whoami`
MINUTE_STAMP=`date +%G`_`date +%m%d`_`date +%H%M`
DATE_STAMP=`date +%m%d`

ADSP_ROOT=/local/mnt/workspace/xinhua/1_SDM630_VIVO_DEMO_801S/ADSP.VT.4.0-00310-SDM660-2/adsp_proc
P4_ROOT=/local/mnt/workspace/xinhua/p4_workspace
OUT_ROOT=/local/mnt/workspace/xinhua/Qualcomm/tmp_801s
OUT_DIR=$OUT_ROOT/$DATE_STAMP

ALGO_VERSION=14430788
CONFIG_VERSION=14355681
FW_VERSION=14359711
UTF_VERSION=40b7ba26b3945fdf64941040c42ecbc36110969d

################################################################################
# 1. Prepare code
################################################################################
# sudo mount -t cifs -o vers=2.1,rw,dir_mode=0777,file_mode=0777,username="helingh",password="qualcomM0" //helingh02.ap.qualcomm.com/Public /media/helingh02

# Prepare UTF
rm -rf $ADSP_ROOT/touch
git clone https://github.qualcomm.com/SCTT/unified_fwk/ $ADSP_ROOT/touch
cd $ADSP_ROOT/touch
git checkout remotes/origin/touch.dsp.2.0_vivo_demo
git checkout $UTF_VERSION
# Prepare algo
rm -rf $P4_ROOT/source/qcom/qct/multimedia2/mmSWAP/Touch/Systems/CSIM_unified
p4 sync -fq //source/qcom/qct/multimedia2/mmSWAP/Touch/Systems/CSIM_unified/...@$ALGO_VERSION
rm -rf $ADSP_ROOT/touch/algo/*
cp -r $P4_ROOT/source/qcom/qct/multimedia2/mmSWAP/Touch/Systems/CSIM_unified/inc $P4_ROOT/source/qcom/qct/multimedia2/mmSWAP/Touch/Systems/CSIM_unified/src $ADSP_ROOT/touch/algo/

cd $ADSP_ROOT/touch/
cscope -Rbkq

# Prepare config
rm -rf $P4_ROOT/source/qcom/qct/multimedia2/mmSWAP/Touch/Config/JSON-Data-Format/Dev/Qualcomm-QRD-SDM630-ADSP-Truly-Truly-QTC801S-v1
p4 sync -fq //source/qcom/qct/multimedia2/mmSWAP/Touch/Config/JSON-Data-Format/Dev/Qualcomm-QRD-SDM630-ADSP-Truly-Truly-QTC801S-v1/...@$CONFIG_VERSION
cp /local/mnt/workspace/xinhua/Qualcomm/DataParser/* $P4_ROOT/source/qcom/qct/multimedia2/mmSWAP/Touch/Config/JSON-Data-Format/Dev/Qualcomm-QRD-SDM630-ADSP-Truly-Truly-QTC801S-v1/
cd $P4_ROOT/source/qcom/qct/multimedia2/mmSWAP/Touch/Config/JSON-Data-Format/Dev/Qualcomm-QRD-SDM630-ADSP-Truly-Truly-QTC801S-v1/
mono DataParser.exe --json2binary --output=my.dat *.json
rm -rf License-Internal-2018.improveTouch-license 

# Prepare FW
rm -rf $P4_ROOT/source/qcom/qct/multimedia2/mmSWAP2/touch/imager/firmware/katmai_prime/Release
p4 sync -fq //source/qcom/qct/multimedia2/mmSWAP2/touch/imager/firmware/katmai_prime/Release/qtc801s.bin@$FW_VERSION

################################################################################
# 2. Compile image
################################################################################
# Prepare UTF

# Compile code
#cd $ADSP_ROOT/touch
# git apply ~/bin/script/hbtp_utils.diff
cd $ADSP_ROOT
patch  touch/algo/inc/hbtp_utils.h < patch.diff
python build/build.py -c sdm660 -f ADSP -o clean
python build/build.py -c sdm660 -f ADSP
cd $ADSP_ROOT/touch/Transport
./build.sh 660 qtc2

################################################################################
# 3. Output folder
################################################################################
rm -rf $OUT_DIR
mkdir -p $OUT_DIR/dsp/build $OUT_DIR/fw
cp -r $OUT_ROOT/common/ap $OUT_DIR/
cp -r $P4_ROOT/source/qcom/qct/multimedia2/mmSWAP/Touch/Config/JSON-Data-Format/Dev/Qualcomm-QRD-SDM630-ADSP-Truly-Truly-QTC801S-v1 $OUT_DIR/config
cp -r $ADSP_ROOT/build/dynamic_signed $OUT_DIR/dsp/build/
mv $OUT_DIR/dsp/build/dynamic_signed/660.adsp.prod/shared_obj/libFastRPC_UTF_Forward_skel.so $OUT_DIR/dsp/build/dynamic_signed/660.adsp.prod/shared_obj/libFastRPC_UTF_Forward_skel.so.old
cp $ADSP_ROOT/touch/Transport/hexagon_ReleaseG_dynamic/signed/libFastRPC_UTF_Forward_skel.so $OUT_DIR/dsp/build/dynamic_signed/660.adsp.prod/shared_obj/libFastRPC_UTF_Forward_skel.so 
cp -r $ADSP_ROOT/obj $OUT_DIR/dsp/
cp -r $P4_ROOT/source/qcom/qct/multimedia2/mmSWAP2/touch/imager/firmware/katmai_prime/Release/qtc801s.bin $OUT_DIR/fw/
cp -r $OUT_ROOT/common/tools $OUT_DIR/
sed "s/DATE_STAMP/$DATE_STAMP/g" $OUT_ROOT/common/flash_all.bat > $OUT_DIR/flash_all.bat

echo "Release        $DATE_STAMP" >> $OUT_DIR/readme.txt
echo "ALGO_VERSION   p4 sync -f //source/qcom/qct/multimedia2/mmSWAP/Touch/Systems/CSIM_unified/...@$ALGO_VERSION" >> $OUT_DIR/readme.txt
echo "CONFIG_VERSION p4 sync -f //source/qcom/qct/multimedia2/mmSWAP/Touch/Config/JSON-Data-Format/Dev/Qualcomm-QRD-SDM630-ADSP-Truly-Truly-QTC801S-v1/...@$CONFIG_VERSION" >> $OUT_DIR/readme.txt
echo "FW_VERSION     p4 sync -f //source/qcom/qct/multimedia2/mmSWAP2/touch/imager/firmware/katmai_prime/Release/qtc801s.bin@$FW_VERSION" >> $OUT_DIR/readme.txt
echo "UTF_VERSION    git clone https://github.qualcomm.com/SCTT/unified_fwk && git checkout remotes/origin/ touch.dsp.2.0_vivo_demo && git checkout $UTF_VERSION" >> $OUT_DIR/readme.txt

cp -rf $OUT_DIR /media/helingh02/code/release/801s/

#sudo mount -t cifs -o vers=2.1,rw,dir_mode=0777,file_mode=0777,username="helingh",password="qualcomM0" //helingh.ap.qualcomm.com/tmp /media/helingh/
#cp -rf $OUT_DIR /media/helingh/801s/
