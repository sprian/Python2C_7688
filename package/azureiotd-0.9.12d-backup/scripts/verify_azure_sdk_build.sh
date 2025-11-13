#!/bin/sh
AZSDK_DIR="${1:-./build-mt7688}"
echo "Listing important libs under $AZSDK_DIR:"
for d in iothub_client provisioning_client c-utility umqtt uhttp deps/parson; do
  if [ -d "$AZSDK_DIR/$d" ]; then
    echo "== $d =="
    res=`ls -1 "${AZSDK_DIR}/${d}" | grep -E 'lib.*\.(a|so)$' || true`
    echo $res
  fi
done
