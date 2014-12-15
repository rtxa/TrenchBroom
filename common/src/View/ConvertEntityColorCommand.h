/*
 Copyright (C) 2010-2014 Kristian Duske
 
 This file is part of TrenchBroom.
 
 TrenchBroom is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.
 
 TrenchBroom is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with TrenchBroom. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef __TrenchBroom__ConvertEntityColorCommand__
#define __TrenchBroom__ConvertEntityColorCommand__

#include "Color.h"
#include "Model/EntityAttributes.h"
#include "Model/ModelTypes.h"
#include "View/DocumentCommand.h"
#include "View/EntityColor.h"

#include <map>

namespace TrenchBroom {
    namespace View {
        class MapDocumentCommandFacade;
        
        class ConvertEntityColorCommand : public DocumentCommand {
        public:
            static const CommandType Type;
        private:
            Model::AttributeName m_attributeName;
            ColorRange::Type m_colorRange;
            
            Model::EntityAttribute::Map m_snapshots;
        public:
            static ConvertEntityColorCommand* convert(const Model::AttributeName& attributeName, ColorRange::Type colorRange);
        private:
            ConvertEntityColorCommand(const Model::AttributeName& attributeName, ColorRange::Type colorRange);
            
            bool doPerformDo(MapDocumentCommandFacade* document);
            bool doPerformUndo(MapDocumentCommandFacade* document);
            
            bool doIsRepeatable(MapDocumentCommandFacade* document) const;
            
            bool doCollateWith(UndoableCommand* command);
        };
    }
}

#endif /* defined(__TrenchBroom__ConvertEntityColorCommand__) */
