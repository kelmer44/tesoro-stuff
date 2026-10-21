/* ScummVM - Graphic Adventure Engine
 *
 * ScummVM is the legal property of its developers, whose names
 * are too numerous to list here. Please refer to the COPYRIGHT
 * file distributed with this source distribution.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "base/plugins.h"
#include "engines/advancedDetector.h"
static const PlainGameDescriptor tesoroGames[] = {{"tesoro", "El Tesoro de Isla Alcachofa"}, {nullptr, nullptr}};
static const ADGameDescription tesoroDescriptions[] = {
	{"tesoro", "Office prototype", AD_ENTRY2s("TESORO.TRE", "40bd64d7df589c2100c529f7a3033087", 67788, "TESORO.DAT", "8fb27fd8a3b31915053372e9d9f7dded", 263612966), Common::ES_ESP, Common::kPlatformWindows, ADGF_UNSTABLE, GUIO1(GUIO_NOMIDI)},
	AD_TABLE_END_MARKER};
class TesoroDetection : public AdvancedMetaEngineDetection<ADGameDescription> {
public:
	TesoroDetection() : AdvancedMetaEngineDetection(tesoroDescriptions, tesoroGames) {}
	const char *getName() const override { return "tesoro"; }
	const char *getEngineName() const override { return "El Tesoro de Isla Alcachofa (prototype)"; }
	const char *getOriginalCopyright() const override { return "El Tesoro de Isla Alcachofa (C) Alcachofa Soft"; }
};
REGISTER_PLUGIN_STATIC(TESORO_DETECTION, PLUGIN_TYPE_ENGINE_DETECTION, TesoroDetection);
