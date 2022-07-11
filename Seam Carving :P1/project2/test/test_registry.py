import os

carve_path = "bin/carve_opt"

def unit_test(tu, test_name):
    test_bin = tu.join_base("bin/testrunner")
    if not os.path.exists(test_bin):
        return tu.FAILURE("testrunner binary not available")
    run_res = tu.run(test_bin, [test_name])
    return tu.check(run_res, "unit test failed")

# expects the program to fail on the input
def test_invalidinput(tu, tn, args):
    carve_bin = tu.join_base(carve_path)
    if not os.path.exists(carve_bin):
        return tu.FAILURE("carve binary not available")
    rc, out, err = tu.run(carve_bin, args)
    check_res = tu.check((rc, out, err), 'application did not return EXIT_FAILURE\n' + err, exp_rc=1)
    if check_res:
        return tu.SUCCESS()
    else:
        return check_res

def test_literal(tu, tn, args_ref_msg):
    carve_bin = tu.join_base(carve_path)
    if not os.path.exists(carve_bin):
        return tu.FAILURE("carve binary not available")
    args, ref_file, msg = args_ref_msg
    rc, out, err = tu.run(carve_bin, args)
    check_res = tu.check((rc, out, err), 'application did not return EXIT_SUCCESS\n' + err)
    if check_res:
        with open(ref_file, "r") as ref_src:
            ref_str = ref_src.read()
            if ref_str == out:
                return tu.SUCCESS()
            else:
                return tu.FAILURE(msg)
    else:
        return check_res

def img_cmp(img1_name, img2_name):
    with open(img1_name, 'r') as img1_src:
        with open(img2_name, 'r') as img2_src:
            img1 = "".join(img1_src.read().split())
            img2 = "".join(img2_src.read().split())
            return img1 == img2

def test_carve(tu, tn, args_ref_out):
    args, ref_file, out_file = args_ref_out
    carve_bin = tu.join_base(carve_path)
    if not os.path.exists(carve_bin):
        return tu.FAILURE("carve binary not available")
    rc, out, err = tu.run(carve_bin, args)
    check_res = tu.check((rc, out, err), 'application did not return EXIT_SUCCESS\n' + err)
    if check_res:
        if not os.access(out_file, os.R_OK):
            return tu.FAILURE('no "out.ppm" produced\n')
        if img_cmp(out_file, ref_file):
            return tu.SUCCESS()
        else:
            return tu.FAILURE('incorrect output image')
    else:
        return check_res


def specialize(fun, arg):
    return lambda tu, tn, x=arg: fun(tu, tn, x)

pre_tests = {
    'public.statistics.small1',
    'public.statistics.small2',
    'public.statistics.owl',
    'public.min_path.small1',
    'public.min_path.small2',
    'public.min_path.owl',
    'public.carve.small1_0',
    'public.carve.small1_1',
    'public.carve.small2_0',
    'public.carve.small2_1',
}

# unit tests are added directly
all_tests = {
    'public.statistics.brightness_small2': unit_test,
    'public.min_path.diff_color': unit_test,
    'public.min_path.energy_small2': unit_test,
    'public.min_path.energy_wide': unit_test,
    'public.min_path.min_energy_wide_1': unit_test,
    'public.min_path.optimal_path_tall': unit_test,
    'public.carve.carve_path_small2': unit_test,
}

for t in pre_tests:
    cat, ex, case = t.split('.', 2)
    catex = cat + '.' + ex
    case_path = 'test/data/' + case + '.ppm'
    ref_prefix = 'test/ref_output/'
    if ex == 'statistics':
        if (case.startswith('imgbroken')):
            all_tests[t] = specialize(test_invalidinput, ['-s', case_path])
        else:
            all_tests[t] = specialize(test_literal, (['-s', case_path], ref_prefix + case + '.statistics', 'incorrect statistics'))
    elif ex == 'min_path':
        all_tests[t] = specialize(test_literal, (['-p', case_path], ref_prefix + case + '.path', 'incorrect minimal path'))
    else:
        assert(ex == 'carve')
        case_file, num = case.split('_', 1)
        case_path = 'test/data/' + case_file + '.ppm'
        all_tests[t] = specialize(test_carve, (['-n', num, case_path], 'test/ref_output/' + case + '.ppm', 'out.ppm'))


timeout_secs = 5

