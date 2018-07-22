/* -*- Mode: C++; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * Copyright (C) Flamewing 2011-2015 <flamewing.sonic@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published
 * by the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>

#include <getopt.h>

#include "kosinski.hh"

using namespace std;

static void usage(char *prog) {
	cerr << "Usage: " << prog << " [-c|--crunch|-x|--extract=[{pointer}]] [-m|--moduled] [-p|--padding=[{len}]] {input_filename} {output_filename}" << endl;
	cerr << endl;
	cerr << "\t-x,--extract\tExtract from {pointer} address in file." << endl;
	cerr << "\t-c,--crunch \tAssume input file is Kosinski-compressed and recompress to output file." << endl
	     << "\t            \tIf --chunch is in effect, a missing output_filename means recompress" << endl
	     << "\t            \tto input_filename. All parameters affect only the output file, except" << endl
	     << "\t            \tfor the -m parameter, which makes both input and output files moduled" << endl
	     << "\t            \t(but the optional module size affects only the output file)." << endl;
	cerr << "\t-m,--moduled\tUse compression in modules of 4096 bytes." << endl;
	cerr << "\t-p|--padding\tFor moduled compression only. Changes internal module padding to {len}." << endl
	     << "\t            \tEach module will be padded to a multiple of the given number; use 1 for" << endl
	     << "\t            \tno padding. Must be a power of 2 (default: " << moduled_kosinski::ModulePadding << ")." << endl << endl;
}

int main(int argc, char *argv[]) {
	static option long_options[] = {
		{"extract", optional_argument, nullptr, 'x'},
		{"moduled", no_argument      , nullptr, 'm'},
		{"crunch" , no_argument      , nullptr, 'c'},
		{"padding", required_argument, nullptr, 'p'},
		{nullptr, 0, nullptr, 0}
	};

	bool extract = false, moduled = false, crunch = false;
	size_t pointer = 0ull,
	       padding = moduled_kosinski::ModulePadding;

	while (true) {
		int option_index = 0;
		int c = getopt_long(argc, argv, "x::mcr:s:p:",
		                    static_cast<option*>(long_options), &option_index);
		if (c == -1) {
			break;
		}

		switch (c) {
			case 'x':
				extract = true;
				if (optarg != nullptr) {
					pointer = strtoul(optarg, nullptr, 0);
				}
				break;
			case 'c':
				crunch = true;
				break;
			case 'm':
				moduled = true;
				break;
			case 'p':
				if (optarg != nullptr) {
					padding = strtoul(optarg, nullptr, 0);
				}
				if ((padding == 0u) || (padding & (padding - 1)) != 0) {
					padding = 16;
				}
				break;
		}
	}

	if ((!crunch && argc - optind < 2) || (crunch && argc - optind < 1)) {
		usage(argv[0]);
		return 1;
	}

	if (extract && crunch) {
		cerr << "Error: --extract and --crunch can't be used at the same time." << endl << endl;
		return 4;
	}

	char *outfile = crunch && argc - optind < 2 ? argv[optind] : argv[optind + 1];

	ifstream fin(argv[optind], ios::in | ios::binary);
	if (!fin.good()) {
		cerr << "Input file '" << argv[optind] << "' could not be opened." << endl << endl;
		return 2;
	}

	if (crunch) {
		stringstream buffer(ios::in | ios::out | ios::binary);
		fin.seekg(pointer);
		if (moduled) {
			kosinski::moduled_decode(fin, buffer, padding);
		} else {
			kosinski::decode(fin, buffer);
		}
		fin.close();
		buffer.seekg(0);

		fstream fout(outfile, ios::in | ios::out | ios::binary | ios::trunc);
		if (!fout.good()) {
			cerr << "Output file '" << argv[optind + 1] << "' could not be opened." << endl << endl;
			return 3;
		}
		if (moduled) {
			kosinski::moduled_encode(buffer, fout, padding);
		} else {
			kosinski::encode(buffer, fout);
		}
	} else {
		fstream fout(outfile, ios::in | ios::out | ios::binary | ios::trunc);
		if (!fout.good()) {
			cerr << "Output file '" << argv[optind + 1] << "' could not be opened." << endl << endl;
			return 3;
		}

		if (extract) {
			fin.seekg(pointer);
			if (moduled) {
				kosinski::moduled_decode(fin, fout, padding);
			} else {
				kosinski::decode(fin, fout);
			}
		} else {
			if (moduled) {
				kosinski::moduled_encode(fin, fout, padding);
			} else {
				kosinski::encode(fin, fout);
			}
		}
	}

	return 0;
}
