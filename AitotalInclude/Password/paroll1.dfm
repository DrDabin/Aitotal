object FParoll: TFParoll
  Left = 495
  Top = 315
  BorderIcons = [biSystemMenu]
  BorderStyle = bsDialog
  Caption = #1042#1074#1086#1076' '#1087#1072#1088#1086#1083#1103' '#1082' '#1072#1088#1093#1080#1074#1091
  ClientHeight = 129
  ClientWidth = 329
  Color = clBtnFace
  Font.Charset = RUSSIAN_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  Position = poDesigned
  PixelsPerInch = 96
  TextHeight = 13
  object EdParoll: TEdit
    Left = 8
    Top = 8
    Width = 313
    Height = 24
    Font.Charset = RUSSIAN_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = 'Microsoft Sans Serif'
    Font.Style = []
    ParentFont = False
    ParentShowHint = False
    ShowHint = True
    TabOrder = 0
    TextHint = #1042#1074#1077#1076#1080#1090#1077' '#1087#1072#1088#1086#1083#1100
  end
  object BtOk: TButton
    Left = 20
    Top = 79
    Width = 97
    Height = 25
    Caption = 'Ok'
    TabOrder = 1
    OnClick = BtOkClick
  end
  object CHPassword: TCheckBox
    Left = 8
    Top = 49
    Width = 97
    Height = 17
    Caption = #1057#1082#1088#1099#1090#1100' '#1087#1072#1088#1086#1083#1100
    TabOrder = 2
    OnClick = CHPasswordClick
  end
  object StatusBar1: TStatusBar
    Left = 0
    Top = 110
    Width = 329
    Height = 19
    Panels = <
      item
        Width = 50
      end>
  end
  object Button1: TButton
    Left = 192
    Top = 79
    Width = 93
    Height = 25
    Caption = #1054#1090#1084#1077#1085#1072
    TabOrder = 4
    OnClick = Button1Click
  end
end
