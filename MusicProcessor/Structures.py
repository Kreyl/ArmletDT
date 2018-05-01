#!/usr/bin/python
#
# Basic data structures for processing scripts for "Dark Tower: All Hail" LARP.
#
from collections import OrderedDict
from re import compile as reCompile

from CSVable import CSVfileable
from Settings import currentTime, getFileName, CHARACTER_ID_START

class CSVdumpable(CSVfileable):
    NEEDS_HEADER = True

    HEADER_COMMENT = '''\

%s.csv for "Dark Tower: All Hail" LARP.

%s

!!! DO NOT EDIT !!!

Generated at %s

'''

    INSTANCES = None
    FILE_NAME_PATTERN = '%s.csv'
    TITLE = None
    GENERATOR = None
    HEADER_TITLE = None

    LIST_PATTERN = reCompile(r'(?<!{){%(.*?)}')

    @classmethod
    def getInstances(cls):
        return cls.INSTANCES.itervalues() if isinstance(cls.INSTANCES, dict) else cls.INSTANCES

    @classmethod
    def getFileName(cls):
        return getFileName(cls.FILE_NAME_PATTERN % cls.TITLE)

    @classmethod
    def getHeaderComment(cls):
        ret = cls.HEADER_COMMENT
        return ret % (cls.TITLE, cls.HEADER_TITLE + (('\n\nGenerated by %s.py' % cls.GENERATOR) if cls.GENERATOR else ''), currentTime()) if ret else None

    @classmethod
    def fillTemplate(cls, templateFileName, target, params = ()):
        with open(getFileName(templateFileName), 'rb') as f:
            template = f.read()
        result = cls.LIST_PATTERN.sub(lambda m: '\n'.join((m.group(1) % v.__dict__) for v in cls.INSTANCES.itervalues()), template)
        result = result % params
        with open(getFileName(target), 'wb') as f:
            f.write(result)

class Source(CSVdumpable):
    CSV_FIELDS = ('sID', 'rName')

    C_NODE = ' /* %s */ { %s, %d, %d, %d },'
    H_NODE = '#define REASON_%s%s %2d'
    COMMENT = '/* %s */'
    COMMENT_OFFSET = 40
    INDENT = '    '

    INSTANCES = []
    TITLE = 'Sources'
    HEADER_TITLE = 'Used to build Sources to Reasons (sID->rID) global map.'
    GENERATOR = 'EmotionProcessor'

    def __init__(self, sID, rName):
        CSVdumpable.__init__(self)
        self.sID = sID
        self.rName = rName

    def sortKey(self):
        return self.sID

class Reason(CSVdumpable):
    CSV_FIELDS = ('rID', 'rName', 'level', 'timeout', 'doganAmount', 'eID', 'eName')

    H_TARGET = 'reasons.h'
    H_TEMPLATE = 'reasons_h.tpl'

    #C_TARGET = 'reasons.cpp'
    #C_TEMPLATE = 'reasons_cpp.tpl'

    INSTANCES = OrderedDict()
    TITLE = 'Reasons'
    HEADER_TITLE = 'Used to build Reasons (rID<->rName) global map and Reasons to Emotions (rID->eID) global map.'
    GENERATOR = 'EmotionProcessor'

    CHARACTER_PRIORITY = 80

    TOP_PRIORITY = 100

    PLACEHOLDER_NAME = 'PLACEHOLDER_%02d'

    def __init__(self, rName, nSources, level, timeout, doganAmount, eName):
        CSVdumpable.__init__(self)
        self.rID = None
        self.rName = rName
        self.nSources = nSources
        self.level = level
        self.timeout = timeout
        self.doganAmount = doganAmount
        self.eID = None
        self.eName = eName

    def sortKey(self):
        return self.rName

    @classmethod
    def sortByIDs(cls):
        return cls._sort(lambda value: value.rID)

    @classmethod
    def addReason(cls, reason):
        cls.INSTANCES[reason.rName] = reason
        return reason

    @classmethod
    def addPlaceholders(cls):
        start = sum(1 for reason in cls.INSTANCES.itervalues() if reason.rID is None)
        assert start <= CHARACTER_ID_START
        for rID in xrange(start, CHARACTER_ID_START):
            reason = Reason(cls.PLACEHOLDER_NAME % rID, 0, 0, 0, 0, None)
            reason.rID = rID
            cls.addReason(reason)

    @classmethod
    def addCharacters(cls, characters):
        for character in characters:
            if character.shortName in cls.INSTANCES:
                cls.INSTANCES[character.shortName].rID = character.rID
            else:
                reason = Reason(character.shortName, 0, 0, 0, 0, None)
                reason.rID = character.rID
                cls.addReason(reason)

    @classmethod
    def dumpCPP(cls):
        params = {'currentTime': currentTime()}
        cls.fillTemplate(cls.H_TEMPLATE, cls.H_TARGET, params)
        #cls.fillTemplate(cls.C_TEMPLATE, cls.C_TARGET, params)

class Emotion(CSVdumpable):
    CSV_FIELDS = ('eID', 'eName', 'eType', 'ePriority', 'isPlayer')

    C_NODE = ' /* %s */ %s{ %s, 1, %s, -1, -1 },'
    H_NODE = '#define EMOTION_%s%s %2d'

    INSTANCES = OrderedDict()
    TITLE = 'Emotions'
    HEADER_TITLE = 'Emotions (eID<->eName) global map.'
    GENERATOR = 'EmotionProcessor'

    def __init__(self, eName, eType, ePriority, isPlayer):
        CSVdumpable.__init__(self)
        self.eID = None
        self.eName = eName
        self.eType = eType
        self.ePriority = ePriority
        self.isPlayer = isPlayer

    def sortKey(self):
        return self.eName

    @classmethod
    def addEmotion(cls, emotion):
        cls.INSTANCES[emotion.eName] = emotion
        return emotion
