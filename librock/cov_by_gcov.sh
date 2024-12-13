mkdir -p product/cov
rm -rf product/cov/*
echo "start copy process files ........"
cp -rf build/test/CMakeFiles/*.dir product/cov
echo "finish copy process files ........"
cd product/cov
files=$(ls)
for file in $files; do
    echo "start analysis "$file" ......."
    cd $file
    lcov -c -d . -o "test.info" -b .
    genhtml -o result test.info
    echo "remove useless files ......."
    rm -f *
    rm -rf __
    cd ..
    echo "finish analysis" $file "........"
done