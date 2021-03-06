
%define name            tpm-tools
%define version         1.3.4
%define release         1

Name:           %{name}
Version:        %{version}
Release:        %{release}
Summary:        Management tools for the TPM hardware

Group:          Applications/System
License:        CPL
URL:            http://www.sf.net/projects/trousers
Source0:        %{name}-%{version}.tar.gz
BuildRoot:      %{_tmppath}/%{name}-%{version}-%{release}-root-%(%{__id_u} -n)

BuildRequires:  autoconf automake libtool trousers-devel opencryptoki-devel openssl-devel
Requires:       trousers

%description
tpm-tools is a group of tools to manage and utilize the Trusted Computing
Group's TPM hardware. TPM hardware can create, store and use RSA keys
securely (without ever being exposed in memory), verify a platform's
software state using cryptographic hashes and more.

%package pkcs11
Summary:	Data management tools that use a PKCS#11 interface to the TPM
Group:		Applications/Productivity
Requires:	%{name} = %{version}-%{release}, opencryptoki >= 2.2.4

%description pkcs11
%{name}-pkcs11 is a group of tools that uses the TPM PKCS#11 token
developed in the opencryptoki project.  All data contained in the
PKCS#11 data store is protected by the TPM (keys, certificates, etc.).
You can import keys and certificates, list out the objects in the data
store, and protect data.

%package devel
Summary:	Files to use the library routines supplied with tpm-tools
Group:		Development/Libraries
Requires:	%{name} = %{version}-%{release} 

%description devel
%{name}-devel is a package that contains the libraries and headers
necessary for developing tpm-tools applications.


%prep
%setup -q


%build
%configure --disable-static --prefix=/usr --libdir=%{_libdir}
make %{?_smp_mflags}


%install
rm -rf $RPM_BUILD_ROOT
make install DESTDIR=$RPM_BUILD_ROOT
rm -f $RPM_BUILD_ROOT/%{_libdir}/libtpm_unseal.la


%clean
rm -rf $RPM_BUILD_ROOT


%files
%defattr(-,root,root,-)
%doc LICENSE README
%attr(755, root, root) %{_bindir}/tpm_*
%attr(755, root, root) %{_sbindir}/tpm_*
#%attr(755, root, root) %{_libdir}/libtpm_unseal.a
%attr(755, root, root) %{_libdir}/libtpm_unseal.so.?.?.?
%{_libdir}/libtpm_unseal.so.0
%{_mandir}/man1/tpm_*
%{_mandir}/man8/tpm_*

%files pkcs11
%defattr(-,root,root,-)
%attr(755, root, root) %{_bindir}/tpmtoken_*
%{_mandir}/man1/tpmtoken_*

%files devel
%defattr(-,root,root,-)
%{_libdir}/libtpm_unseal.so
%{_includedir}/tpm_tools/*.h
%{_mandir}/man3/tpmUnseal*


%post -p /sbin/ldconfig


%postun -p /sbin/ldconfig

%changelog
* Fri Nov 16 2007 Kent Yoder <kyoder@users.sf.net> - 1.3.1
- Updates to configure
* Fri Oct 05 2007 Kent Yoder <kyoder@users.sf.net> - 1.2.5.1
- Updated %build section to use smp_mflags

