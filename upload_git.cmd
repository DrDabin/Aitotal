:<<EOF

@echo off
SetLocal EnableExtensions
cd /d "%~dp0" || cd /d "%~dps0"

echo.
echo --------------------------------
echo GitHub Uploader by Alex Dragokas
echo --------------------------------
echo.

"C:\Program Files\Git\bin\sh.exe" --login -i -- "%~nx0"
pause
exit /b
EOF

repo="Aitotal"

git config --global user.name "DrDabin"
git config --global user.email "Aibolit198@mail.ru"
#git config --global user.signingkey B024668D57E188FF
git config --global core.autocrlf false
git config --global core.safecrlf false

# If repository is already initialized with at least one file
# Warning !!! All files pushed by contributors will be overwritten by local version
rm -R -f .git
rm -R -f GitHub-tmp
mkdir GitHub-tmp
cd GitHub-tmp
git clone git@github.com:DrDabin/$repo.git

mv $repo/.git ../
cd ..
rm -R -f GitHub-tmp

# This is required for the first commit (to create .git folder locally)
if [ ! -d ".git" ]; then
  git init
  git remote add origin git@github.com:dragokas/$repo.git
fi

git checkout master

# This is required if somebody pushed to your repository
# git pull origin master --allow-unrelated-histories

echo ""
echo ""

git add *

echo ""
echo ""

echo "Ready to upload ..."
echo ""
echo -e "ENTER NEW VERSION AND COMMENT IN FILE version_comment.txt AND PRESS ENTER: \c"

read -r line
echo ""

#git commit -a -m "$line"
git commit -a -F version_comment.txt

echo ""
echo ""
git push -u origin master
echo ""
