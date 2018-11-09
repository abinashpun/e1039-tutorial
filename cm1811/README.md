## Tutorial at E1039 18/11 Collaboration Meeting

### Log in to seaquestgpvm01

```bash
kinit -f <usr>@FNAL.GOV
```
type your password, then

```bash
ssh -AKX -l krbtgt/FNAL.GOV@FNAL.GOV <usr>@seaquestgpvm01.fnal.gov
```

### source the setup macro

```bash
source /e906/app/users/yuhw/setup.sh
```

### git clone the tutorial repository

If you have a GitHub account and have your publick key uploaded to GitHub.

Refer [Connecting to GitHub with SSH](https://help.github.com/articles/connecting-to-github-with-ssh/)
and [this E1039 wiki page](https://github.com/E1039-Collaboration/e1039-wiki/wiki/Work-with-GitHub)
```bash
git clone git@github.com:E1039-Collaboration/e1039-tutorial.git
```

If not try
```bash
source /grid/fermiapp/products/uboone/setup_uboone.sh
setup git
source /e906/app/users/yuhw/setup.sh
git clone https://github.com/E1039-Collaboration/e1039-tutorial.git
```
