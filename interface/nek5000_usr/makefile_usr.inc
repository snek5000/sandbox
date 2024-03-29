# vim: set ft=make noexpandtab
$(OBJDIR)/frame.o             :toolbox/frame.f toolbox/FRAMELP
	$(F77) -c $(FL2) -I./ $< -o $@

$(OBJDIR)/mntrlog_block.o     :toolbox/mntrlog_block.f toolbox/MNTRLOGD
	$(F77) -c $(FL2) -I./ $< -o $@

$(OBJDIR)/mntrlog.o           :toolbox/mntrlog.f toolbox/MNTRLOGD
	$(F77) -c $(FL2) -I./ $< -o $@

$(OBJDIR)/mntrtmr_block.o     :toolbox/mntrtmr_block.f toolbox/MNTRLOGD toolbox/MNTRTMRD
	$(F77) -c $(FL2) -I./ $< -o $@

$(OBJDIR)/mntrtmr.o           :toolbox/mntrtmr.f toolbox/MNTRLOGD toolbox/MNTRTMRD toolbox/FRAMELP
	$(F77) -c $(FL2) -I./ $< -o $@

$(OBJDIR)/rprm_block.o        :toolbox/rprm_block.f toolbox/RPRMD
	$(F77) -c $(FL2) -I./ $< -o $@

$(OBJDIR)/rprm.o              :toolbox/rprm.f toolbox/RPRMD toolbox/FRAMELP
	$(F77) -c $(FL2) -I./ $< -o $@

$(OBJDIR)/io_tools_block.o    :toolbox/io_tools_block.f toolbox/IOTOOLD
	$(F77) -c $(FL2) -I./ $< -o $@

$(OBJDIR)/io_tools.o          :toolbox/io_tools.f toolbox/IOTOOLD
	$(F77) -c $(FL2) -I./ $< -o $@

$(OBJDIR)/chkpoint.o          :toolbox/chkpoint.f toolbox/CHKPOINTD
	$(F77) -c $(FL2) -I./ $< -o $@

$(OBJDIR)/chkpt_mstp.o        :toolbox/chkpt_mstp.f toolbox/CHKPTMSTPD toolbox/CHKPOINTD
	$(F77) -c $(FL2) -I./ $< -o $@

$(OBJDIR)/map2D.o             :toolbox/map2D.f toolbox/MAP2D toolbox/FRAMELP
	$(F77) -c $(FL2) -I./ $< -o $@

$(OBJDIR)/stat.o              :toolbox/stat.f toolbox/STATD toolbox/MAP2D toolbox/FRAMELP
	$(F77) -c $(FL2) -I./ $< -o $@

$(OBJDIR)/stat_IO.o           :toolbox/stat_IO.f toolbox/STATD toolbox/MAP2D toolbox/FRAMELP
	$(F77) -c $(FL2) -I./ $< -o $@

$(OBJDIR)/math_tools.o        :toolbox/math_tools.f
	$(F77) -c $(FL2) -I./ $< -o $@

libnek5000.so                 :objdir $(NOBJS) usrfile
	$(FC) -shared $(NOBJS) $(CASEDIR)/${CASENAME}.o -lc $(USR_LFLAGS) -o $@

ffi: setup_ffi.py libnek5000.so
	python $<

cython: setup_cy.py nek5000_cy.pyx libnek5000.so
	python $< build_ext --inplace

pybind: nek5000_pybind.cpp
	mpicxx -Wall -c $<
	gcc -Wall -fPIC -c add.c
	gcc -shared add.o -o libadd.so
	mpicxx -Wall -L. -lnek5000 -ladd nek5000_pybind.o -o nek5000_$@

cleanso:
	rm -f nek5000*.so
	rm -f *_{cy,ffi}.{c,o}
	rm -rf build
