TESTS = \
	csvproj_help_test.sh \
	csvproj_p0_test.sh \
	csvproj_p1_test.sh \
	csvsort_k1x_test.sh \
	csvjoin_jtx_test.sh \
	csvfilter_r1x_test.sh \
	csvaggr_a0x_test.sh \
	csvconv_4_test.sh \
	csvpivot_test.sh \
	cmdline_test.sh

dist_check_DATA= \
	csvproj_help.txt.in csvproj_version.txt.in csvproj_ignore.txt \
	t0.csv \
	t0_proj0.csv t0_proj1.csv \
	t0_sort1d.csv t0_sort1dn.csv t0_sort1n.csv t0_sort1x.csv \
	t1.csv \
	t1_aggr0cc.csv t1_aggr0cnt.csv t1_aggr0mmrev.csv t1_aggr0mn.csv t1_aggr0mx.csv t1_aggr0s.csv t1_aggr0first.csv t1_aggr0last.csv \
	t1_filt01.csv t1_filt02.csv t1_filt1a.csv t1_filtne.csv \
	t1_sort1e0d.csv \
	t1t2_natural.csv t1t2_outer.csv \
	t2.csv t2esc.csv t2esch.csv t3.csv t2t3_cross.csv \
	t4.csv \
	t4_all.csv t4_preserve.csv t4_remove.csv t4_resolve.csv t4_xrm.csv \
	t5.csv t5_sort1n.csv t5_aggr1smm.csv \
	t6.csv t6pivot_sum.csv t6pivot_concat.csv t6pivot_count.csv t6pivot_first.csv t6pivot_last.csv t6pivot_max.csv t6pivot_min.csv

EXTRA_DIST=$(TESTS)

AM_TESTS_ENVIRONMENT=export "PACKAGE_STRING=@PACKAGE_STRING@";
