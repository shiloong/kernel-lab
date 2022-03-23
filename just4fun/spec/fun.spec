Summary: A trial kernel module just for fun
Name: fun
Version: %{rpm_version}
Release: %{rpm_release}%{?dist}
Group: Development/Libraries
License: GPL
URL: http://www.aliyun.com
Source0: %{name}-%{version}.tar.gz
Source1: %{name}.modules

BuildRoot: %{_tmppath}/%{name}-%{version}-%{release}-root
Autoreq: 0
%description
This is a trial out-of-tree kernel module just for fun.

%global nproc %(getconf _NPROCESSORS_ONLN)

%prep
rm -rf %{buildroot}

%setup -q

%build
make -j%{nproc} -C /lib/modules/%{kernel_version}/build M=$(pwd) modules

%install
mkdir -p $RPM_BUILD_ROOT/lib/modules/%{kernel_version}/updates/
install -m755 fun.ko $RPM_BUILD_ROOT/lib/modules/%{kernel_version}/updates/

mkdir -p $RPM_BUILD_ROOT/etc/sysconfig/modules
install -c -m755 %{SOURCE1} $RPM_BUILD_ROOT/etc/sysconfig/modules/

%post
depmod -A

%preun

%postun
depmod -A

%clean
rm -rf $RPM_BUILD_ROOT

%files
%defattr(-,root,root)

/lib/modules/*/updates/fun.ko
/etc/sysconfig/modules/fun.modules

%changelog
