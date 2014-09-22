#!/bin/sh

# Generate HiWiFi App tarball from current directory.

generate_app()
{
	local revision=`date +%Y%m%d.%H%M`
	local appdir=`pwd`
	local appname=`basename "$appdir"`
	local uploadfiles=""
	local tarfiles=""

	local file
	for file in *; do
		if [ -f "$file" ]; then
			case "$file" in
				*.tar.gz|*.tgz) : ;;  # ignore other tarballs
				*) uploadfiles="$uploadfiles$file ";;
			esac
		elif [ -d "$file" ]; then
			echo "Creating $file.tar.gz for $file/ ..."
			( cd "$file"; tar zcf ../"$file".tar.gz --exclude-vcs * )
			uploadfiles="$uploadfiles$file.tar.gz "
			tarfiles="$tarfiles$file.tar.gz "
		fi
	done

	tar zcvf ${appname}-${revision}.tgz $uploadfiles

	[ -n "$tarfiles" ] && rm -f $tarfiles
	return 0
}

generate_app
