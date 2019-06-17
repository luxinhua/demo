#!/bin/bash
# arg1=start, arg2=end, format: %s.%N  

SERVER_ADDR=$(ping review-android.quicinc.com -c1 | sed '1{s/[^(]*(//;s/).*//;q}') 

function getTiming() 
{  
    start=$1  
    end=$2  
     
    start_s=$(echo $start | cut -d '.' -f 1)  
    start_ns=$(echo $start | cut -d '.' -f 2)  
    end_s=$(echo $end | cut -d '.' -f 1)  
    end_ns=$(echo $end | cut -d '.' -f 2)  
  
  
# for debug..  
#    echo $start  
#    echo $end  
  
  
    time=$(( ( 10#$end_s - 10#$start_s ) * 1000 + ( 10#$end_ns / 1000000 - 10#$start_ns / 1000000 ) ))  
  
  
    echo "Git CherryPick used $time ms"  
}  
  


usage ()
{
	echo "==============================="
	echo "Usage: $0 -p root_path -a (apply|push) -f|c (filename| changes)"
	echo "==============================="
}

changes=
if [ $# -lt 6 ]; then
	echo "Error:please check your arguments"
	usage
	exit 1 
fi

if [ "$1" = "-p"  -a \( -d "$2" -a -w "$4" \) ]; then
	echo "Error:please check your root path"
	usage
	exit 1
fi
root_path=$2
shift
shift
if [ ! "$1" = "-a" ] && [ ! \( "$2" = "push" -o "$2" = "apply" \) ]; then
	echo "Error:please check your action and parameter"
	usage
	exit 1
fi
shift
shift

if [ "$1" = "-f" -a \( -f "$2" -a -r "$4" \) ] || [ "$1" = "-c" -a "$2" != "" ]; then
	shift
	changes=$@
else
	echo "Error:please check your changes and parameter"
	usage
	exit 1
fi

#check the local have the repo dir
pushd $root_path 2>&1 >/dev/null
if [ ! -f ./.repo/manifest.xml ];then
	echo "Error: can not find the repo dir"
#	exit 1
fi

#get QUALPASS for curl get patch#
echo "Please input your QUALPASS:"
read -s PASSWD
#echo $PASSWD

 
for change in $changes
do
	start=$(date +%s.%N) 
	echo $change | grep "/" >/dev/null 2>&1
	if [ $? = 0 ];then
		#patchset=$(echo $change | awk -F '/' '{print $NF}')
		patchset=${change##*/}
		echo $change
		change=${change%%/*}
		change_patch=$change.$patchset.patch
		echo "--------https://review-android.quicinc.com/#/c/$change/$patchset---------"

#		ssh -p 29418 $USER@$SERVER_ADDR gerrit query $change --current-patch-set > project_info
		ssh -p 29418 $USER@$SERVER_ADDR gerrit query $change --patch-sets > project_info
	#	project_nfull=$(ssh -p 29418 $USER@review-android.quicinc.com gerrit query $change --current-patch-set)
                patchset_found=0
                parent_num=0
		while read line 
		do
			if [ $patchset_found = 0 ];then
				echo $line | grep "number: $patchset" >/dev/null 2>&1
			        if [ $? = 0 ];then
			  		#echo "found given patchset $patchset"
			                patchset_found=1
			                continue
			        fi
			else
        			echo $line | grep "revision:" >/dev/null 2>&1
				if [ $? = 0 ];then
				revision=$(echo $line | grep revision | awk '{print $2}')
				#echo "inr#$revision"
				continue
				fi

        			echo $line | grep "parents:" >/dev/null 2>&1
				if [ $? = 0 ];then
				((parent_num++))
				#echo "inp#$parent_num"
				continue
				fi

				echo $line | grep "ref:" >/dev/null 2>&1
				if [ $? = 0 ];then
				change_refs=$(echo $line | grep ref | awk '{print $2}')
				#echo "inc#$change_ref"
				break 
				fi
			fi

		done < project_info
		
		project_name=$(cat project_info | grep "project:" | awk '{print $2}')
		project_path=$(./.repo/repo/repo list $project_name | awk '{print $1}')
		pushd $project_path 2>&1 >/dev/null
	 else
		echo "--------https://review-android.quicinc.com/#/c/$change---------"
		ssh -p 29418 $USER@$SERVER_ADDR gerrit query $change --current-patch-set > project_info
	#	project_name=$(ssh -p 29418 $USER@review-android.quicinc.com gerrit query $change --current-patch-set | grep project | awk '{print $2}')
		project_name=$(cat project_info | grep "project:" | awk '{print $2}')
		change_refs=$(cat project_info | grep "ref:" | awk '{print $2}')
		patchset=$(echo $change_refs | awk -F '/' '{print $NF}')
		change_patch=$change.$patchset.patch
		revision=$(cat project_info | grep "revision:" | awk '{print $2}')
		parent_num=$(cat project_info | grep parents -wc)
	#	project_path=$(.repo/repo/repo forall $project_name -c 'echo $REPO_PATH' 2>/dev/null)
	#	project_path=$(.repo/repo/repo list $(project_name) | awk '{print $1}')
		project_path=$(./.repo/repo/repo list $project_name | awk '{print $1}')
		pushd $project_path 2>&1 >/dev/null
	#	change_refs=$(ssh -p 29418 $USER@review-android.quicinc.com gerrit query $change --current-patch-set | grep "ref:" | awk '{print $2}')
	fi

	# Check wether it is a merge change
	#parent_num=$(ssh -p 29418 $USER@review-android.quicinc.com gerrit query $change --current-patch-set | grep parents -wc)
	#parent_num=$(cat project_info | grep parents -wc)
	if [ $parent_num -gt 1 ];then
		echo "++It's a Merge Change"
		echo "++ git pull ssh://review-android.quicinc.com:29418/$project_name $change_refs"
		pull_info=$(git pull ssh://$SERVER_ADDR:29418/$project_name $change_refs)
		pull_status=$?
		if [ $pull_status -ne 0 ]; then
			echo "+++ ERROR: ================================"
			echo "@@@        you changes $change pull failed, retrun code($pull_status)"
			echo "+++        message: $pull_info"
			echo "+++ ERROR: ================================"
			echo "--------------------------------------------------------------"
			echo ""
			echo "STOP Because Git Pull failed"
			end=$(date +%s.%N)  
			getTiming $start $end
		exit 

			continue
		else
			echo "=======================Git Pull $change_refs Succeed======================"
			end=$(date +%s.%N)  
			getTiming $start $end
		fi
		popd 2>&1 >/dev/null
		echo "--------------------------------------------------------------"
		echo ""
		
	else
		echo "++ get patch from https://git.quicinc.com/?p=$project_name.git;a=patch;h=$revision"
#		echo "/usr/bin/curl -u $USER:$PASSWD -o $change_patch -k -L \"https://git.quicinc.com/?p=$project_name.git;a=patch;h=$revision\""
		getpatch_info=$(/usr/bin/curl -u $USER:$PASSWD -o $change_patch -k -L "https://git.quicinc.com/?p=$project_name.git;a=patch;h=$revision" 2>/dev/null)
		getpatch_status=$?
	
		if [ $getpatch_status -ne 0 ]; then
			echo "+++ ERROR: ================================"
			echo "@@@        you changes $change get patch failed, retrun code($getpatch_status)"
			echo "+++        message: $getpatch_info"
			echo "+++ ERROR: ================================"
			echo "--------------------------------------------------------------"
			echo ""
			echo "Retry Again"
			echo "++ get patch from https://git.quicinc.com/?p=$project_name.git;a=patch;h=$revision"
			getpatch_info=$(/usr/bin/curl -u $USER:$PASSWD -o $change_patch -k -L "https://git.quicinc.com/?p=$project_name.git;a=patch;h=$revision" 2>/dev/null)
			getpatch_status=$?
			if [ $getpatch_status -ne 0 ]; then
			        echo "+++ ERROR: ================================"
			        echo "@@@        you changes $change get patch failed, retrun code($getpatch_status)"
			        echo "+++        message: $getpatch_info"
			        echo "+++ ERROR: ================================"
			        echo "--------------------------------------------------------------"
			        echo ""
			        echo "Stop because get patch from git.quicinc.com failed, Please check your network"
				end=$(date +%s.%N)  
				getTiming $start $end

				exit 
			fi
		fi
		echo "++ git am --ignore-space-change --ignore-whitespace $change_patch"
		rm -rf .git/rebase-apply
		applypatch_info=$(git am --ignore-space-change --ignore-whitespace $change_patch 2>&1 >/dev/null)
		applypatch_status=$?
		if [ $applypatch_status -ne 0 ]; then
			echo "$applypatch_info" 
			echo "++++ ERROR: ================================"
			echo "@@@@ you changes $change apply patch failed, return code($applypatch_status)"
			echo "++++ Message: $applypatch_info"
			conflict=$(git status -s)
			echo "++++ $conflict"
			echo "++++ ERROR: ================================"
			#echo "STOP Because git apply patch failed"
			git am --abort
			ChangeId=$(grep Change-Id $change_patch | grep Change-Id | awk '{print $2}')
			if git log | grep $ChangeId >/dev/null 2>&1; then
				echo "#######################Please ignore the previous ERROR#########################"
				echo "#######################Change already in the base code##########################"
			else
				#Use previous git cherrypick method to apply change
				echo ""
				echo "Use traditional git cherrypick"
				echo "++ git fetch ssh://review-android.quicinc.com:29418/$project_name $change_refs"
				fetch_info=$(git fetch ssh://$SERVER_ADDR:29418/$project_name $change_refs)
				if [ $fetch_status -ne 0 ]; then
				        echo "+++ ERROR: ================================"
				        echo "@@@        you changes $change fetch failed, retrun code($fetch_status)"
				        echo "+++        message: $fetch_info"
				        echo "+++ ERROR: ================================"
				        echo "--------------------------------------------------------------"
				        echo ""
				        echo "STOP Because Git Fetch failed"
				exit
				        continue
				fi
				echo "++ git cherry-pick  FETCH_HEAD"
				cherrypick_info=$(git cherry-pick FETCH_HEAD 2>&1 >/dev/null)
				cherrypick_status=$?
				if [ $cherrypick_status -ne 0 ]; then
				        echo "$cherrypick_info" | grep "allow-empty" 2>&1 >/dev/null
				        if [ $? -eq 0 ];then
				                echo "++++ Warning: this change is allow-empty"
				        else
				                echo "++++ ERROR: ================================"
				                echo "@@@@ you changes $change cherrypick failed, return code($cherrypick_status)"
				                echo "++++ Message: $cherrypick_info"
				                conflict=$(git status -s)
				                echo "++++ $conflict"
				                echo "++++ ERROR: ================================"
				                echo "STOP Because Git Cherrypick failed"
				    exit
				        fi
				else
				        echo "===========================Git Cherry Pick Succeed====================="
				fi
			fi
			#else menas Change has already been in the code
		    #exit 
		else
			echo "===========================Git apply patch $change_refs Succeed====================="
		fi
		end=$(date +%s.%N)  
		getTiming $start $end
		#git reset --hard
		popd 2>&1 >/dev/null
		echo "--------------------------------------------------------------"
		echo ""
	fi
done
