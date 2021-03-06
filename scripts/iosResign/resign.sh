
echo  "重签过程请依次正确输入：ipa文件、描述文件mobileprovision、证书名称cerName\n"


read -p "请输入ipa文件路径(拖入终端即可) : " param_ipa
echo "输入:$param_ipa\n"

if ! ([ -f "$param_ipa" ]); then
echo \"${param_ipa}\"文件不存在
exit
fi
 
read -p "请输入mobileprovision文件路径(拖入终端即可) : " param_mobileprovision
echo "输入:$param_mobileprovision\n"

if ! ([ -f "$param_mobileprovision" ]); then
echo \"${param_mobileprovision}\"文件不存在
exit
fi
 
read -p "请输入cerName（'iPhone Developer: XXXXX (XXX)'） : " param_cerName
echo "输入:$param_cerName\n"

 
unzip -qo "$param_ipa" -d extracted
 
APPLICATION=$(ls extracted/Payload/)
 
cp "$param_mobileprovision" "extracted/Payload/$APPLICATION/embedded.mobileprovision"
 
echo "Resigning with certificate: $param_cerName" >&2
find -d extracted  \( -name "*.app" -o -name "*.appex" -o -name "*.framework" -o -name "*.dylib" \) > directories.txt
security cms -D -i "extracted/Payload/$APPLICATION/embedded.mobileprovision" > t_entitlements_full.plist
/usr/libexec/PlistBuddy -x -c 'Print:Entitlements' t_entitlements_full.plist > t_entitlements.plist
while IFS='' read -r line || [[ -n "$line" ]]; do
    /usr/bin/codesign --continue -f -s "$param_cerName" --entitlements "t_entitlements.plist"  "$line"
done < directories.txt

 
echo "Creating the Signed IPA\n"
cd extracted
zip -qry ../extracted.ipa *
cd ..

ls_date=`date +%Y%m%d`
mv extracted.ipa "${ls_date}_Signed.ipa"

echo "重签输出ipa：${ls_date}_Signed.ipa"
open ./

rm -rf "extracted"
rm directories.txt
rm t_entitlements.plist
rm t_entitlements_full.plist