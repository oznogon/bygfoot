%define name bygfoot
%define version UNKNOWN_VERSION
%define release RELEASE
%define configure_prefix CFLAGS=-O3
Release: %{release}
Name: %{name}
Summary: Bygfoot is a football (soccer) manager game
Version: %{version}
Source: bygfoot-%{version}-%{release}.tar.bz2
Group: Amusements/Games
URL: http://bygfoot.sourceforge.net
BuildRoot: /var/tmp/%{name}-buildroot
License: GPL
Prefix: /usr
Requires: zip

%description
Bygfoot is a small and simple football (a.k.a. soccer) manager game
featuring quite a few international leagues and cups. You manage a
team from one such league: you form the team, buy and sell players,
get promoted or relegated and of course try to be successful.
%prep
rm -rf $RPM_BUILD_ROOT
%setup -q
%build
./configure --prefix=%{prefix} %{configure_prefix}
make
%install
make install prefix=$RPM_BUILD_ROOT/%{prefix}
%files
%{prefix}/bin/bygfoot*
%{prefix}/share/bygfoot
%{prefix}/share/locale/*/LC_MESSAGES/bygfoot.mo
%defattr(-,root,root,0755)
