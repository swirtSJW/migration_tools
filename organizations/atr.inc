<?php

/**
 * @file
 * Defines migration classes for Antitrust Division.
 */

/**
 * Node ID of ATR organization.
 */
define('ATR_NID', 1630);

/**
 * Migrates .html files from /atr to page nodes.
 *
 * @package doj_migration
 * @subpackage atr
 */
class AtrPageMigration extends NGJusticeHtmlToPageMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {

    // Define source directories.
    $source_dirs = array(
      0 => 'atr',
      725 => 'atr/contact/jobs',
      726 => 'atr/contact/jobs/paq',
      727 => 'atr/contact',
      728 => 'atr/foia',
      729 => 'atr/foia/divisionmanual',
      730 => 'atr/foia/frito-lay',
      731 => 'atr/foia/rll',
      732 => 'atr/hmerger',
      733 => 'atr/icpac',
      734 => 'atr/public',
      735 => 'atr/public/216254',
      736 => 'atr/public/220241',
      737 => 'atr/public/231424',
      738 => 'atr/public/articles',
      739 => 'atr/public/busreview',
      740 => 'atr/public/comments',
      741 => 'atr/public/comments/sec271',
      742 => 'atr/public/comments/sec271/ameritech',
      743 => 'atr/public/comments/sec271/bellatlantic',
      744 => 'atr/public/comments/sec271/bellsouth',
      745 => 'atr/public/comments/sec271/qwest',
      746 => 'atr/public/comments/sec271/sbc',
      747 => 'atr/public/comments/sec271/verizon',
      748 => 'atr/public/criminal',
      749 => 'atr/public/divisionmanual/supporting_documents',
      750 => 'atr/public/eag',
      751 => 'atr/public/eag/221240',
      752 => 'atr/public/eag/221241',
      753 => 'atr/public/eag/221243',
      754 => 'atr/public/eag/221246',
      755 => 'atr/public/eag/221876',
      756 => 'atr/public/eag/221877',
      757 => 'atr/public/eag/221883',
      758 => 'atr/public/eag/228687',
      759 => 'atr/public/eag/228709',
      760 => 'atr/public/eag/232668',
      761 => 'atr/public/eag/232692',
      762 => 'atr/public/electronic_discovery',
      763 => 'atr/public/guidelines',
      764 => 'atr/public/health_care',
      765 => 'atr/public/health_care/204694',
      766 => 'atr/public/hearings/ip',
      767 => 'atr/public/hearings/single_firm',
      768 => 'atr/public/hearings/single_firm/bio',
      769 => 'atr/public/hearings/single_firm/comments',
      770 => 'atr/public/hearings/single_firm/docs',
      771 => 'atr/public/hearings/single_firm/docs/218780',
      772 => 'atr/public/hearings/single_firm/docs/219980',
      773 => 'atr/public/hearings/single_firm/docs/222104',
      774 => 'atr/public/international',
      775 => 'atr/public/international/docs',
      800 => 'atr/public/real_estate',
      801 => 'atr/public/reports',
      807 => 'atr/public/taskforces',
      808 => 'atr/public/testimony',
      809 => 'atr/public/workshops',
      // 810 => 'atr/public/workshops/ag2010',
      811 => 'atr/public/workshops/airlines2008',
      812 => 'atr/public/workshops/docs',
      813 => 'atr/public/workshops/docs/202661',
      814 => 'atr/public/workshops/ip',
      815 => 'atr/public/workshops/rewbios',
      816 => 'atr/public/workshops/rewcom',
      817 => 'atr/public/workshops/rewcom/213351',
      818 => 'atr/public/workshops/techassist2008',
      819 => 'atr/public/workshops/techassist2008/bios',
      820 => 'atr/public/workshops/telecom2007',
      821 => 'atr/public/workshops/telecom2007/bios',
      822 => 'atr/public/workshops/telecom2007/submissions/227645',
      823 => 'atr/public/workshops/telecom2007/submissions',
      824 => 'atr/public/workshops/telecom2007/submissions/227819',
      825 => 'atr/public/workshops/telecom2007/submissions/227839',
      826 => 'atr/public/workshops/telecom2007/submissions/227842',
      827 => 'atr/public/workshops/telecom2007/submissions/228008',
      828 => 'atr/public/workshops/telecom2007/submissions/228010',
      829 => 'atr/public/workshops/telecom2007/submissions/228096',
      830 => 'atr/public/workshops/telecom2007/submissions/228097',
      831 => 'atr/public/workshops/telecom2007/submissions/228098',
      832 => 'atr/public/workshops/telecom2007/submissions/228099',
      833 => 'atr/public/workshops/telecom2007/submissions/228100',
      834 => 'atr/public/workshops/telecom2007/submissions/228101',
      835 => 'atr/public/workshops/telecom2007/submissions/228972',
      836 => 'atr/public/workshops/telecom2007/submissions/229055',
      837 => 'atr/victim',
      838 => 'atr/about',
      847 => 'atr/espanol',
      848 => 'atr/events/hearings/comphearing',
      849 => 'atr/events/hearings/hchearing',
      850 => 'atr/events',
      851 => 'atr/events/workshops/meworkshop',
      852 => 'atr/public/appellate',
      853 => 'atr/public/closing',
      854 => 'atr/public/division-update/2011',
      855 => 'atr/public/division-update/2011/gallery',
      856 => 'atr/public/division-update/2012',
      857 => 'atr/public/division-update/2012/gallery',
      858 => 'atr/public/division-update/2013',
      859 => 'atr/public/division-update/2014',
      860 => 'atr/public/divisionmanual',
      861 => 'atr/public/guidelines/horiz_book',
      862 => 'atr/public/international/gallery',
      863 => 'atr/public/taskforces/eemtaskforce',
      864 => 'atr/public/update/2010',
      865 => 'atr/public/update/2010/gallery',
      866 => 'atr/public/update',
      867 => 'atr/public/workshops/cpp',
      868 => 'atr/public/workshops/healthcare/2015/02',
      869 => 'atr/public/workshops/mfn',
      870 => 'atr/public/workshops/pae',
      874 => 'atr/rss',
      875 => 'atr/cases/amex',
      876 => 'atr/cases/apple/exhibits',
      877 => 'atr/cases/ascapbmi',
      878 => 'atr/cases/bazaarvoice',
      879 => 'atr/cases/usairways',
    );

    $options = array('recurse' => FALSE);

    $arguments['source_parser_class'] = "AtrPageSourceParser";

    // Configuring Obtainers.
    $title = new ObtainerInfo('title', "AtrObtainTitlePage");
    $title->addMethod('findClassBreadcrumbMenuContentLast');
    $title->addMethod('pluckSelector', array("h1", 1));
    $title->addMethod('pluckSelector', array("title", 1));

    $body = new ObtainerInfo('body', 'AtrObtainBody');
    $body->addMethod('findTopBodyHtml');
    $body->addMethod('findClassContentSub');
    $body->addMethod('pluckSelector', array(".atr-bottom-left", 1));

    $arguments['obtainers_info'][] = $title;
    $arguments['obtainers_info'][] = $body;

    // Parent constructor will set $this->source, $this->destination, and
    // $this->map.
    parent::__construct($arguments, $source_dirs, NULL, $options);
    $this->dependencies = array('Organization');
    $this->description = t('Migrates pages from the Antitrust Division.');
    $this->addFieldMapping('og_group_ref')
    ->defaultValue('atr')
    ->sourceMigration('Organization');
  }

  /**
   * {@inheritdoc}
   */
  public function prepareRow($row) {
    $skip_these = array(
      '/atr/public/divisionmanual/index.htm',
      '/atr/public/appellate/appellate.htm',
      '/atr/victim/vrcomplaint.htm',
      '/atr/victim/vrcontacts.htm',
      '/atr/victim/vrhearings.htm',
      '/atr/victim/vroverview.htm',
      '/atr/victim/vrresources.htm',
      '/atr/victim/vrmain.htm',
      '/atr/public/workshops/telecom2007/panelist_submissions.htm',
      '/atr/public/workshops/telecom2007/index.htm',
      '/atr/public/workshops/telecom2007/press_releases.htm',
      '/atr/public/workshops/telecom2007/speakers.htm',
      '/atr/public/workshops/telecom2007/agenda.htm',
      '/atr/public/workshops/telecom2007/other_submissions.htm',
      '/atr/public/workshops/techassist2008/index.htm',
      '/atr/public/workshops/techassist2008/press.htm',
      '/atr/public/workshops/techassist2008/background.htm',
      '/atr/public/workshops/techassist2008/agenda.htm',
      '/atr/public/workshops/ip/index.htm',
      '/atr/public/workshops/airlines2008/index.htm',
      '/atr/public/workshops/airlines2008/agenda.htm',
      '/atr/public/workshops/ag2010/index.htm',
      '/atr/public/workshops/ag2010/all_comments_alpha.htm',
      '/atr/public/workshops/ag2010/alabama-agenda.htm',
      '/atr/public/workshops/ag2010/initial_comments_topic.htm',
      '/atr/public/workshops/ag2010/initial_comments_alpha.htm',
      '/atr/public/workshops/ag2010/all_comments_num.htm',
      '/atr/public/workshops/ag2010/wisconsin-agenda.htm',
      '/atr/public/workshops/ag2010/agenda.htm',
      '/atr/public/workshops/ag2010/colorado-agenda.htm',
      '/atr/public/workshops/reworkshop.htm',
      '/atr/public/workshops/rewagenda.htm',
      '/atr/public/workshops/reworkshop_rewcomments.htm',
      '/atr/public/workshops/rewpanelists.htm',
      '/atr/public/workshops/rewmajor.htm',
      '/atr/public/workshops/mewagenda2.htm',
      '/atr/public/workshops/rehotels.htm',
      '/atr/public/workshops/reworkshop_atrcomments.htm',
      '/atr/public/workshops/meworkshop.htm',
      '/atr/public/testimony/hhi.htm',
      '/atr/public/testimony/testimon.htm',
      '/atr/public/testimony/othertestimony.htm',
      '/atr/public/taskforces/eemtaskforce.htm',
      '/atr/public/reports/index.htm',
      '/atr/public/real_estate/northcarolina.htm',
      '/atr/public/real_estate/florida.htm',
      '/atr/public/real_estate/index.htm',
      '/atr/public/real_estate/southdakota.htm',
      '/atr/public/real_estate/northdakota.htm',
      '/atr/public/real_estate/california.htm',
      '/atr/public/real_estate/michigan.htm',
      '/atr/public/real_estate/nebraska.htm',
      '/atr/public/real_estate/ohio.htm',
      '/atr/public/real_estate/enforce.htm',
      '/atr/public/real_estate/vermont.htm',
      '/atr/public/real_estate/missouri.htm',
      '/atr/public/real_estate/rhodeisland.htm',
      '/atr/public/real_estate/utah.htm',
      '/atr/public/real_estate/virginia.htm',
      '/atr/public/real_estate/wisconsin.htm',
      '/atr/public/real_estate/hawaii.htm',
      '/atr/public/real_estate/kentucky.htm',
      '/atr/public/real_estate/closing_details.htm',
      '/atr/public/real_estate/mls.htm',
      '/atr/public/real_estate/alabama.htm',
      '/atr/public/real_estate/westvirginia.htm',
      '/atr/public/real_estate/delaware.htm',
      '/atr/public/real_estate/alaska.htm',
      '/atr/public/real_estate/maine.htm',
      '/atr/public/real_estate/indiana.htm',
      '/atr/public/real_estate/feeforservice.htm',
      '/atr/public/real_estate/wyoming.htm',
      '/atr/public/real_estate/rebates.htm',
      '/atr/public/real_estate/rebates_details.htm',
      '/atr/public/real_estate/models.htm',
      '/atr/public/real_estate/idaho.htm',
      '/atr/public/real_estate/minnesota.htm',
      '/atr/public/real_estate/tennessee.htm',
      '/atr/public/real_estate/southcarolina.htm',
      '/atr/public/real_estate/montana.htm',
      '/atr/public/real_estate/mississippi.htm',
      '/atr/public/real_estate/pennsylvania.htm',
      '/atr/public/real_estate/iowa.htm',
      '/atr/public/real_estate/workshop_reports.htm',
      '/atr/public/real_estate/dc.htm',
      '/atr/public/real_estate/illinois.htm',
      '/atr/public/real_estate/louisiana.htm',
      '/atr/public/real_estate/connecticut.htm',
      '/atr/public/real_estate/states.htm',
      '/atr/public/real_estate/commission_table.htm',
      '/atr/public/real_estate/rebate_calculator.htm',
      '/atr/public/real_estate/faqs.htm',
      '/atr/public/real_estate/states_map.htm',
      '/atr/public/real_estate/newyork.htm',
      '/atr/public/real_estate/nevada.htm',
      '/atr/public/real_estate/oklahoma.htm',
      '/atr/public/real_estate/washington.htm',
      '/atr/public/real_estate/massachusetts.htm',
      '/atr/public/real_estate/newhampshire.htm',
      '/atr/public/real_estate/arizona.htm',
      '/atr/public/real_estate/fee_details.htm',
      '/atr/public/real_estate/kansas.htm',
      '/atr/public/real_estate/closing.htm',
      '/atr/public/real_estate/georgia.htm',
      '/atr/public/real_estate/maryland.htm',
      '/atr/public/real_estate/save.htm',
      '/atr/public/real_estate/colorado.htm',
      '/atr/public/real_estate/texas.htm',
      '/atr/public/real_estate/oregon.htm',
      '/atr/public/real_estate/arkansas.htm',
      '/atr/public/real_estate/newjersey.htm',
      '/atr/public/real_estate/newmexico.htm',
      '/atr/public/international/int_arrangements.htm',
      '/atr/public/international/intdocs.htm',
      '/atr/public/international/oecd.htm',
      '/atr/public/international/susocp.htm',
      '/atr/public/international/intl_other.htm',
      '/atr/public/hearings/single_firm/sfcspeeches.htm',
      '/atr/public/hearings/single_firm/sfcjune.htm',
      '/atr/public/hearings/single_firm/sfcmar2007.htm',
      '/atr/public/hearings/single_firm/sfcfeb2007.htm',
      '/atr/public/hearings/single_firm/sfcpress.htm',
      '/atr/public/hearings/single_firm/sfcnov.htm',
      '/atr/public/hearings/single_firm/sfccomments.htm',
      '/atr/public/hearings/single_firm/sfcjan2007.htm',
      '/atr/public/hearings/single_firm/sfcoct.htm',
      '/atr/public/hearings/single_firm/sfchearing.htm',
      '/atr/public/hearings/single_firm/sfctranscripts.htm',
      '/atr/public/hearings/single_firm/sfcjuly.htm',
      '/atr/public/hearings/single_firm/sfcdec.htm',
      '/atr/public/hearings/single_firm/sfcmay2007.htm',
      '/atr/public/hearings/single_firm/sfcsept.htm',
      '/atr/public/health_care/health_care.htm',
      '/atr/public/guidelines/207001.htm',
      '/atr/public/guidelines/207001.htm',
      '/atr/public/guidelines/206542.htm',
      '/atr/public/guidelines/206542.htm',
      '/atr/public/guidelines/207021.htm',
      '/atr/public/guidelines/207021.htm',
      '/atr/public/guidelines/206584.htm',
      '/atr/public/guidelines/206584.htm',
      '/atr/public/guidelines/206936.htm',
      '/atr/public/guidelines/206936.htm',
      '/atr/public/guidelines/206782.htm',
      '/atr/public/guidelines/206782.htm',
      '/atr/public/guidelines/207144.htm',
      '/atr/public/guidelines/207144.htm',
      '/atr/public/guidelines/206826.htm',
      '/atr/public/guidelines/206826.htm',
      '/atr/public/guidelines/4371.htm',
      '/atr/public/guidelines/207102.htm',
      '/atr/public/guidelines/207102.htm',
      '/atr/public/guidelines/206696.htm',
      '/atr/public/guidelines/206696.htm',
      '/atr/public/guidelines/introduc.htm',
      '/atr/public/guidelines/introduc.htm',
      '/atr/public/guidelines/ncrpa.htm',
      '/atr/public/guidelines/jointindex.htm',
      '/atr/public/guidelines/significant_guidance.htm',
      '/atr/public/guidelines/guidelin.htm',
      '/atr/public/electronic_discovery/index.htm',
      '/atr/public/eag/221879a.htm',
      '/atr/public/eag/221873a.htm',
      '/atr/public/eag/255003a.htm',
      '/atr/public/eag/257888a.htm',
      '/atr/public/eag/227376a.htm',
      '/atr/public/eag/232692a.htm',
      '/atr/public/eag/227380a.htm',
      '/atr/public/eag/240245a.htm',
      '/atr/public/eag/225695a.htm',
      '/atr/public/eag/225694a.htm',
      '/atr/public/eag/236664a.htm',
      '/atr/public/eag/221246a.htm',
      '/atr/public/eag/228477a.htm',
      '/atr/public/eag/258801a.htm',
      '/atr/public/eag/221878a.htm',
      '/atr/public/eag/257581a.htm',
      '/atr/public/eag/221241a.htm',
      '/atr/public/eag/discussion_papers_author.htm',
      '/atr/public/eag/07-13a.htm',
      '/atr/index.html',
      '/atr/about/daag.html',
      '/atr/public/eag/221875a.htm',
      '/atr/public/eag/236823a.htm',
      '/atr/public/eag/236662a.htm',
      '/atr/public/eag/240246a.htm',
      '/atr/public/eag/230712a.htm',
      '/atr/public/eag/231577a.htm',
      '/atr/public/eag/221243a.htm',
      '/atr/public/eag/221240a.htm',
      '/atr/public/eag/seminars.htm',
      '/atr/public/eag/221880a.htm',
      '/atr/public/eag/251856a.htm',
      '/atr/public/eag/236663a.htm',
      '/atr/public/eag/260724a.htm',
      '/atr/public/eag/225693a.htm',
      '/atr/public/eag/246374a.htm',
      '/atr/public/eag/236665a.htm',
      '/atr/public/eag/240080a.htm',
      '/atr/public/eag/221876a.htm',
      '/atr/public/eag/232668a.htm',
      '/atr/public/eag/248394a.htm',
      '/atr/public/eag/compad_papers.htm',
      '/atr/public/eag/262643a.htm',
      '/atr/public/eag/248399a.htm',
      '/atr/public/eag/227383a.htm',
      '/atr/public/eag/221881a.htm',
      '/atr/public/eag/251857a.htm',
      '/atr/public/eag/231610a.htm',
      '/atr/public/eag/225696a.htm',
      '/atr/public/eag/discussion_papers.htm',
      '/atr/public/eag/221883a.htm',
      '/atr/public/eag/papers.htm',
      '/atr/public/eag/221242a.htm',
      '/atr/public/eag/246372a.htm',
      '/atr/public/eag/221882a.htm',
      '/atr/public/eag/228687a.htm',
      '/atr/public/eag/228709a.htm',
      '/atr/public/eag/221874a.htm',
      '/atr/public/eag/248396a.htm',
      '/atr/public/eag/244098a.htm',
      '/atr/public/eag/221877a.htm',
      '/atr/public/criminal/leniency.htm',
      '/atr/public/criminal/recovery_resources.htm',
      '/atr/public/criminal/recovery_initiative.htm',
      '/atr/public/criminal/recovery_report.htm',
      '/atr/public/criminal/economic_recovery.htm',
      '/atr/public/criminal/recovery_training.htm',
      '/atr/public/criminal/red_flags_collusion.htm',
      '/atr/public/comments/sec271/sec271.htm',
      '/atr/public/comments/comments_states.htm',
      '/atr/public/comments/comments.htm',
      '/atr/public/busreview/index.htm',
      '/atr/public/busreview/procedure.htm',
      '/atr/public/busreview/letters.htm',
      '/atr/public/busreview/digests.htm',
      '/atr/public/articles/index.htm',
      '/atr/public/criminal.htm',
      '/atr/public/premerger.htm',
      '/atr/public/workstats.htm',
      '/atr/public/comments_testimony.htm',
      '/atr/public/division_operations.htm',
      '/atr/icpac/icpac.htm',
      '/atr/icpac/finalreport.htm',
      '/atr/icpac/transcripts.htm',
      '/atr/foia/202396.htm',
      '/atr/foia/foiapublicdocs.htm',
      '/atr/foia/202489.htm',
      '/atr/foia/foiaroom.htm',
      '/atr/contact/jobs_other.htm',
      '/atr/contact/newcase.htm',
      '/atr/contact/jobs_economists.htm',
      '/atr/contact/otheratr.htm',
      '/atr/contact/jobs_paralegals.htm',
      '/atr/contact/jobs_attorneys.htm',
      '/atr/contact/confpol.htm',
      '/atr/contact/phoneworks.htm',
      '/atr/contact/jobforms.htm',
      '/atr/contact/jobs.htm',
      '/atr/whatsnew.htm',
      '/atr/hchearing.htm',
      '/atr/hearing.htm',
      '/atr/offices2.htm',
      '/atr/disaster.htm',
      '/atr/cases.htm',
      '/atr/hcseptember.htm',
      '/atr/events.htm',
      '/atr/hcfebruary.htm',
      '/atr/hmerger.htm',
      '/atr/hcjune.htm',
      '/atr/sections.htm',
      '/atr/hcapril.htm',
      '/atr/hcmay.htm',
      '/atr/org.htm',
      '/atr/hcmarch.htm',
      '/atr/laws.htm',
      '/atr/google760bbb7d6ce2dfa3.htm',
      '/atr/pubdocs.html',
      '/atr/overview.html',
      '/atr/foia.html',
      '/atr/contact.html',
      '/atr/public/eag/262643.htm',
      '/atr/google760bbb7d6ce2dfa3.html',
      '/atr/cvarneybio.htm',
      '/atr/cases.html',
    );
    if (doj_migration_skip_file($row->fileid, $skip_these) || parent::prepareRow($row) === FALSE) {
      return FALSE;
    }
  }
}

/**
 * Migrates files (*.pdf) from /atr.
 *
 * @package doj_migration
 * @subpackage atr
 */
class AtrFileMigration extends JusticeDeployableBinaryFileMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    $this->description = t('Migrates non-image files from the atr subdirectory.');
    $this->dependencies = array('Organization');

    // Match .pdf files only.
    $source_dirs = array(
      'atr',
    );

    $regex = '/.*\.(pdf|txt|rtf|doc|docx|xls|xlsx|csv|mp3|mp4|wpd|wp|qpw|xml|ppt|pptx|db|rdf|swf|tex|vcf|wav|wps|xht|xml|zip)/i';
    $dest_dir = 'public:///atr/docs';

    $options = array(
      'recurse' => TRUE,
      'nomask' => '/(\.\.?|CVS|cases.*)$/',
    );

    // This will setup $this->map, $this->destination, and $this->source.
    // It will also add field mappings for file location and redirects, which
    // relies on prepareRow() defining $row->filepath and $row->legacy_path.
    parent::__construct($arguments, $source_dirs, $dest_dir, $regex, $options);
  }


  /**
   * {@inheritdoc}
   */
  public function getOrganizationAbbreviation() {
    return "atr";
  }
}

/**
 * Migrates press releases from /atr.
 *
 * @package doj_migration
 * @subpackage atr
 */
class AtrPressReleaseMigration extends NGJusticeHtmlToPressReleaseMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    $arguments['component'] = "Antitrust Division";
    $arguments['source_parser_class'] = "AtrPressSourceParser";
    $arguments['component_tid'] = 376;
    // Configuring Obtainers.
    $title = new ObtainerInfo('title', "AtrObtainTitlePress");
    $title->addMethod('pluckAnySelectorUntilValid', array('h1'));
    $title->addMethod('pluckSelector', array("#contentstart > div > h2", 2));
    $title->addMethod('pluckSelector', array("h2", 1));
    $title->addMethod('pluckSelector', array(".contentSub > div > p[align='center'] > strong", 1));
    $title->addMethod('pluckSelector', array(".contentSub > div > div > p > strong", 1));
    $title->addMethod('pluckSelector', array("#headline", 1));
    $title->addMethod('pluckSelector', array("p > strong > em", 1));
    $title->addMethod('pluckSelector', array("p > strong > u", 1));
    $title->addMethod('pluckSelector', array("#contentstart > div > h2", 1));

    $date = new ObtainerInfo('date', "AtrObtainDate");
    $date->addMethod('findDateInTableAfterBr');
    $date->addMethod('pluckSelector', array("p[align='center']", 1));
    $date->addMethod('pluckSelector', array("#contentstart > p", 1));
    $date->addMethod('pluckSelector', array(".newsRight", 1));
    $date->addMethod('pluckSelector', array(".BottomLeftContent", 1));
    $date->addMethod('pluckProbableDate');

    $pr_number = new ObtainerInfo('prNumber', "AtrObtainPrNumber");
    $pr_number->addMethod('findLastP');

    $arguments['obtainers_info'][] = $title;
    $arguments['obtainers_info'][] = $date;
    $arguments['obtainers_info'][] = $pr_number;

    $source_dirs = array(
      797 => 'atr/public/press_releases/2013',
      798 => 'atr/public/press_releases/2014',
      799 => 'atr/public/press_releases/2015',
    );

    parent::__construct($arguments, $source_dirs);

    $this->addFieldMapping('og_group_ref')
      ->defaultValue('atr')
      ->sourceMigration('Organization');

    // Add file attachment.
    $this->addFieldMapping('field_pr_attachment', 'filename');
    $this->addFieldMapping('field_pr_attachment:file_class')->defaultValue('MigrateFileUri');
    $this->addFieldMapping('field_pr_attachment:source_dir', 'source_dir');
    $this->addFieldMapping('field_pr_attachment:destination_dir', 'destination_dir');
    $this->addFieldMapping('field_pr_attachment:file_replace')->defaultValue(FILE_EXISTS_REUSE);

    $this->addFieldMapping('context')->defaultValue('no_right_sidebar');
  }

  /**
   * {@inheritdoc}
   */
  public function prepareRow($row) {
    parent::prepareRow($row);

    // Generate filepath based on article date.
    $time_created = strtotime($row->field_pr_date);
    $year_created = date('Y', $time_created);
    $month_created = date('m', $time_created);
    $date_created = date('d', $time_created);

    // Set up our attachment-related arrays.
    $row->attachmentRedirects = array();
    $row->filename = array();
    $row->source_dir = array();
    $row->destination_dir = array();

    // An array of pdfLinks is created when the source parse is initialized,
    // in AtrPressReleaseSourceParser::cleanHtml().
    foreach ($this->sourceParser->fileLinks as $file_link) {
      // Add redirects for the attachment's legacy URL.
      $row->attachmentRedirects[] = $file_link;
      $row->filename[] = basename($file_link);
      $row->source_dir[] = variable_get("doj_migration_base_dir") . '/' . dirname($file_link) . '/';
      $row->destination_dir[] = "public:///atr/press_release/$year_created/$month_created/$date_created";
    }
  }
}

/**
 * Class AtrIndustryMigration.
 */
class AtrIndustryMigration extends JusticeCsvMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    $source_file = __DIR__ . '/../sources/industry_codes.csv';
    $options = array('header_rows' => TRUE);
    parent::__construct($arguments, $source_file, $options);

    $this->description = t('Migrates industries for ATR cases.');
    $this->destination = new MigrateDestinationTerm('industry', array('allow_duplicate_terms' => TRUE));
    $this->map = new MigrateSQLMap($this->machineName,
      array(
        'code' => array(
          'type' => 'varchar',
          'length' => 255,
          'not null' => TRUE,
          'description' => 'Topic ID',
        ),
      ),
      MigrateDestinationTerm::getKeySchema()
    );

    $this->addFieldMapping('name', 'name');
    $this->addFieldMapping('field_industry_code', 'code');
  }

  /**
   * {@inheritdoc}
   */
  public function csvcolumns() {
    // Note: Remember to subtract 1 from column number.
    $columns[0] = array('name', 'Industry Name');
    $columns[1] = array('code', 'Industry Code');

    return $columns;
  }

  /**
   * {@inheritdoc}
   */
  public function fields() {
    // This is used by the migration analysis tool and is not necessary
    // for a functioning migration.
    return array();
  }
}

/**
 * Class AtrCsvMigration.
 *
 * This is the parent class of both AtrCaseMigration and
 * AtrCaseDocumentMigration.
 */
abstract class AtrCsvMigration extends JusticeCsvToNodeMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments, $source_file, $options) {
    /*
    // @codingStandardsIgnoreStart
    $this->addFieldMapping('og_group_ref')->defaultValue('atr')->sourceMigration('Organization');
    $this->addFieldMapping('workbench_moderation_state_new')->defaultValue('published');

    // Create redirects using relative, legacy path of static file.
    $this->addFieldMapping('migrate_redirects', 'legacy_path')->separator('|');
    $this->addSimpleMappings(array('title', 'uid'));
    // @codingStandardsIgnoreEnd
    */
    parent::__construct($arguments, $source_file, $options);
  }

  /**
   * Returns the array of CSV columns in the case file.
   */
  public function caseCsvColumns() {
    // Note: Remember to subtract 1 from column number.
    $columns[0] = array('index_url', 'Case Index URL');
    $columns[1] = array('title', 'Case/Party Name');
    $columns[2] = array('date_opened', 'Date Opened');
    $columns[3] = array('date_closed', 'Date Closed');
    $columns[4] = array('matter_type', 'Matter Type');
    $columns[5] = array('court', 'Court');
    $columns[6] = array('industry', 'Industry');
    $columns[7] = array('codes', 'NAIC/SIC Code');
    $columns[8] = array('violation', 'Violation');
    $columns[9] = array('market', 'Market');
    $columns[10] = array('keywords', 'Case Keywords');

    return $columns;
  }

  /**
   * Returns the array of CSV columns in the case documents file.
   */
  public function caseDocumentCsvColumns() {
    $columns = array();

    // Note: Remember to subtract 1 from column number.
    $columns[0] = array('index_url', 'Case Index URL');
    $columns[1] = array('document_url', 'Document URL');
    $columns[2] = array('date', 'Document Date');
    $columns[3] = array('title', 'Document Title');
    $columns[4] = array('type', 'Document Type');
    $columns[5] = array('keywords', 'Document Keywords');

    return $columns;
  }

  /**
   * Maps a type value to the corresponding select list key.
   *
   * @param string $field_name
   *   The machine name of the field for which to find keys.
   *
   * @param string $value
   *   The value from the source.
   *
   * @return string
   *   The corresponding select list key, or else the raw value.
   */
  protected function mapValueToSelectListKey($field_name, $value) {
    $field = field_info_field($field_name);
    $allowed_values = list_allowed_values($field);
    foreach ($allowed_values as $key => $allowed_value) {
      if ($allowed_value == $value) {
        return $key;
      }
    }

    return FALSE;
  }
}

/**
 * Class AtrCaseMigration.
 *
 * @package doj_migration
 * @subpackage atr
 */
class AtrCaseMigration extends AtrCsvMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    $component_tid = 376;
    $this->baseDir = variable_get('doj_migration_base_dir', DOJ_LEGACY_WEBROOT);
    $source_file = __DIR__ . '/../sources/atr-cases.csv';
    $options = array('header_rows' => TRUE);
    parent::__construct($arguments, $source_file, $options);

    $this->dependencies = array(
      'Organization',
      'AtrIndustry',
      'AtrCaseDocument',
    );
    $this->description = t('Migrates ATR Cases.');

    // Create a map object for tracking the relationships between source rows.
    $this->map = new MigrateSQLMap($this->machineName,
      array(
        'index_url' => array(
          'type' => 'varchar',
          'length' => 255,
          'not null' => TRUE,
          'description' => 'Title',
        ),
      ),
      MigrateDestinationNode::getKeySchema()
    );

    $this->destination = new MigrateDestinationNode('case');

    // Define non-simple field mappings.
    $this->addFieldMapping('field_case_body', 'body');
    $this->addFieldMapping('field_case_body:format')->defaultValue('wysiwyg');
    $this->addFieldMapping('field_case_date', 'date_opened');
    $this->addFieldMapping('field_case_date:to', 'date_closed');
    $this->addFieldMapping('field_case_type', 'matter_type')->callbacks(array($this, 'mapTypeToAllowedValue'));
    $this->addFieldMapping('field_brief_federal_court', 'court')->callbacks(array($this, 'translateCourtValues'));
    $this->addFieldMapping('field_brief_court_level')->defaultValue('district_court');
    $this->addFieldMapping('field_case_market', 'market');
    $this->addFieldMapping('field_case_documents', 'case_document_ids')->sourceMigration('AtrCaseDocument');
    $this->addFieldMapping('field_case_violation', 'violation')->separator('|');
    $this->addFieldMapping('field_case_violation:create_term')->defaultValue(TRUE);
    $this->addFieldMapping('field_case_violation:ignore_case')->defaultValue(FALSE);
    $this->addFieldMapping('field_case_industry', 'codes')->separator('|')->callbacks(array($this, 'getIndustryTermByCode'));
    $this->addFieldMapping('field_case_industry:source_type')->defaultValue('tid');
    $this->addFieldMapping('field_case_component')->defaultValue(array($component_tid));
    $this->addFieldMapping('field_case_component:source_type')->defaultValue('tid');

    // $this->addFieldMapping('field_case_tags', 'keywords');
    $this->addFieldMapping('field_case_tags:create_term')->defaultValue(TRUE);
    $this->addFieldMapping('field_case_tags:ignore_case')->defaultValue(FALSE);

    $this->addFieldMapping('field_case_name', 'title');

    $this->addPathDnmMapping(array(
      'field_case_topic',
      'field_case_overview',
      'field_case_link',
      'field_case_attachments',
      'field_case_images',
      'field_case_teaser',
    ));
  }

  /**
   * {@inheritdoc}
   */
  public function csvcolumns() {
    return $this->caseCsvColumns();
  }

  /**
   * {@inheritdoc}
   */
  public function fields() {
    // This is used by the migration analysis tool and is not necessary
    // for a functioning migration.
    return array();
  }

  /**
   * Maps a type value to the corresponding select list key.
   *
   * @param string $value
   *   The value from the source.
   *
   * @return string
   *   The corresponding select list key, or else the raw value.
   */
  protected function mapTypeToAllowedValue($value) {
    if ($key = $this->mapValueToSelectListKey('field_case_type', $value)) {
      return $key;
    }

    $this->migrationMessage("Could not find allowed value for @field_name, source value was '@value',", array(
      '@field_name' => 'field_case_type',
      '@value' => $value,
    ), WATCHDOG_WARNING);

    return $value;
  }

  /**
   * Returns the tids of industry terms, given an industry codes.
   *
   * @param array $codes
   *   An array of industry codes.
   *
   * @return array
   *   An array of taxonomy term ids.
   */
  protected function getIndustryTermByCode($codes) {
    $tids = array();
    foreach ($codes as $code) {
      // Skip empty codes.
      if (!$code) {
        continue;
      }

      $query = new EntityFieldQuery();
      $query->entityCondition('entity_type', 'taxonomy_term')
        ->entityCondition('bundle', 'industry')
        ->fieldCondition('field_industry_code', 'value', $code, '=')
        ->addMetaData('account', user_load(1));
      $result = $query->execute();

      if (isset($result['taxonomy_term'])) {
        $term_ids = array_keys($result['taxonomy_term']);
        $tid = reset($term_ids);

        $tids[] = $tid;
      }
      else {
        $this->migrationMessage('Could not find corresponding industry term for code "@code" in "@file".', array(
          '@code' => $code,
          '@file' => $this->sourceValues->index_url,
        ), WATCHDOG_ERROR);
      }
    }

    return $tids;
  }

  /**
   * Returns the corresponding field value for an incorrectly labeled court.
   *
   * @param string $value
   *   The original, incorrect value.
   *
   * @return string
   *   The corrected value, or else the original if none can be found.
   */
  protected function translateCourtValues($value) {
    // Not all values are wrong! Try first.
    if ($key = $this->mapValueToSelectListKey('field_brief_federal_court', $value)) {
      return $key;
    }

    $map = array(
      'District of Rhode Island' => 'Rhode Island, District of',
      'District of Puerto Rico' => 'Puerto Rico, District of',
      'District of New Hampshire' => 'New Hampshire, District of',
      'District of Massachusetts' => 'Massachusetts, District of',
      'District of Maine' => 'Maine, District of',
      'District of Vermont' => 'Vermont, District of',
      'Western District of New York' => 'New York, Western District',
      'Southern District of New York' => 'New York, Southern District',
      'Northern District of New York' => 'New York, Northern District',
      'Eastern District of New York' => 'New York, Eastern District',
      'District of Connecticut' => 'Connecticut, District of',
      'District of Delaware' => 'Delaware, District of',
      'District of New Jersey' => 'New Jersey, District of',
      'Eastern District of Pennsylvania' => 'Pennsylvania, Eastern District',
      'Middle District of Pennsylvania' => 'Pennsylvania, Middle District',
      'Western District of Pennsylvania' => 'Pennsylvania, Western District',
      'District of the Virgin Islands' => 'Virgin Islands, District of',
      'District of Maryland' => 'Maryland, District of',
      'Eastern District of North Carolina' => 'North Carolina, Eastern District',
      'Middle District of North Carolina' => 'North Carolina, Middle District',
      'Western District of North Carolina' => 'North Carolina, Western District',
      'District of South Carolina' => 'South Carolina, District of',
      'Eastern District of Virginia' => 'Virginia, Eastern District',
      'Western District of Virginia' => 'Virginia, Western District',
      'Northern District of West Virginia' => 'West Virginia, Northern District',
      'Southern District of West Virginia' => 'West Virginia, Southern District',
      'Eastern District of Louisiana' => 'Louisiana, Eastern District',
      'Middle District of Louisiana' => 'Louisiana, Middle District',
      'Western District of Louisiana' => 'Louisiana, Western District',
      'Northern District of Mississippi' => 'Mississippi, Northern District',
      'Southern District of Mississippi' => 'Mississippi, Southern District',
      'Eastern District of Texas' => 'Texas, Eastern District',
      'Northern District of Texas' => 'Texas, Northern District',
      'Southern District of Texas' => 'Texas, Southern District',
      'Western District of Texas' => 'Texas, Western District',
      'Eastern District of Kentucky' => 'Kentucky, Eastern District',
      'Western District of Kentucky' => 'Kentucky, Western District',
      'Eastern District of Michigan' => 'Michigan, Eastern District',
      'Western District of Michigan' => 'Michigan, Western District',
      'Northern District of Ohio' => 'Ohio, Northern District',
      'Southern District of Ohio' => 'Ohio, Southern District',
      'Eastern District of Tennessee' => 'Tennessee, Eastern District',
      'Middle District of Tennessee' => 'Tennessee, Middle District',
      'Western District of Tennessee' => 'Tennessee, Western District',
      'Eastern District of Illinois' => 'Illinois, Eastern District',
      'Northern District of Illinois' => 'Illinois, Northern District',
      'Central District of Illinois' => 'Illinois, Central District',
      'Southern District of Illinois' => 'Illinois, Southern District',
      'Northern District of Indiana' => 'Indiana, Northern District',
      'Southern District of Indiana' => 'Indiana, Southern District',
      'Eastern District of Wisconsin' => 'Wisconsin, Eastern District',
      'Western District of Wisconsin' => 'Wisconsin, Western District',
      'Eastern District of Arkansas' => 'Arkansas, Eastern District',
      'Western District of Arkansas' => 'Arkansas, Western District',
      'Northern District of Iowa' => 'Iowa, Northern District',
      'Southern District of Iowa' => 'Iowa, Southern District',
      'District of Minnesota' => 'Minnesota, District of',
      'Eastern District of Missouri' => 'Missouri, Eastern District',
      'Western District of Missouri' => 'Missouri, Western District',
      'District of Nebraska' => 'Nebraska, District of',
      'District of North Dakota' => 'North Dakota, District of',
      'District of South Dakota' => 'South Dakota, District of',
      'District of Alaska' => 'Alaska, District of',
      'District of Arizona' => 'Arizona, District of',
      'Central District of California' => 'California, Central District',
      'Eastern District of California' => 'California, Eastern District of',
      'Northern District of California' => 'California, Northern District',
      'Southern District of California' => 'California, Southern District',
      'District of Guam' => 'Guam, District of',
      'District of Hawaii' => 'Hawaii, District of',
      'District of Idaho' => 'Idaho, District of',
      'District of Montana' => 'Montana, District of',
      'District of Nevada' => 'Nevada, District of',
      'District of the Northern Mariana Islands' => 'Northern Mariana Islands, District of the',
      'District of Oregon' => 'Oregon, District of',
      'Eastern District of Washington' => 'Washington, Eastern District',
      'Western District of Washington' => 'Washington, Western District',
      'District of Colorado' => 'Colorado, District of',
      'District of Kansas' => 'Kansas, District of',
      'District of New Mexico' => 'New Mexico, District of',
      'Eastern District of Oklahoma' => 'Oklahoma, Eastern District',
      'Northern District of Oklahoma' => 'Oklahoma, Northern District',
      'Western District of Oklahoma' => 'Oklahoma, Western District',
      'District of Utah' => 'Utah, District of',
      'District of Wyoming' => 'Wyoming, District of',
      'Northern District of Alabama' => 'Alabama, Northern District',
      'Middle District of Alabama' => 'Alabama, Middle District',
      'Southern District of Alabama' => 'Alabama, Southern District',
      'Northern District of Florida' => 'Florida, Northern District',
      'Middle District of Florida' => 'Florida, Middle District',
      'Southern District of Florida' => 'Florida, Southern District',
      'Northern District of Georgia' => 'Georgia, Northern District',
      'Middle District of Georgia' => 'Georgia, Middle District',
      'Southern District of Georgia' => 'Georgia, Southern District',
      'District of District of Columbia' => 'District of Columbia, District of',
    );

    // Whoops.
    $flipped_array = array_flip($map);

    if (!empty($flipped_array[$value])) {
      $new_value = $flipped_array[$value];
      if ($key = $this->mapValueToSelectListKey('field_brief_federal_court', $new_value)) {
        return $key;
      }
    }

    $this->migrationMessage("Could not find allowed value for field_brief_federal_court, source value was '@value' in '@file'", array(
      '@value' => $value,
      '@file' => $this->sourceValues->index_url,
    ), WATCHDOG_WARNING);

    return $value;
  }

  /**
   * {@inheritdoc}
   */
  public function prepareRow($row) {
    parent::prepareRow($row);

    $system_file_path = str_replace('http://www.justice.gov', $this->baseDir, $row->index_url);

    try {
      $content = file_get_contents($system_file_path);
    }
    catch (Exception $e) {
      $this->migrationMessage("Could not load file '@file'", array('@file' => $system_file_path), WATCHDOG_ERROR);
      return FALSE;
    }

    $parsed_url = parse_url($row->index_url);
    $row->url_path = $parsed_url['path'];
    $row->legacy_path = trim($row->url_path, '/');

    // Check whether a redirect to this page already exists, if so, do not
    // migrate the "duplicate" content.
    if ($this->isDuplicateByRedirect($row)) {
      return FALSE;
    }

    $source_parser = new AtrCaseSourceParser($row->legacy_path, $content);
    $row->body = $source_parser->getBody();

    // Set the case documents to the source ids used in
    // AtrCaseDocumentMigration.
    $row->case_document_ids = $this->getCaseDocumentIds($row->index_url);

    // Set to admin.
    $row->uid = 1;
  }

  /**
   * Gets the ids (abs URLS) of all case documents associated with a case.
   *
   * @param string $index_url
   *   The URL of the case index page.
   *
   * @return array
   *   An array of case document ids (absolute urls).
   */
  protected function getCaseDocumentIds($index_url) {
    $this->baseDir = variable_get('doj_migration_base_dir', DOJ_LEGACY_WEBROOT);
    $source_file = __DIR__ . '/../sources/atr-case-documents.csv';
    $options = array('header_rows' => TRUE);
    $atr_case_documents = new MigrateSourceCSV($source_file, $this->caseDocumentCsvColumns(), $options);

    $ids = array();
    $atr_case_documents->performRewind();
    while ($row = $atr_case_documents->getNextRow()) {
      if ($row->index_url == $index_url) {
        $ids[] = $row->document_url;
      }
    }

    return $ids;
  }
}

/**
 * This source parser is used to parse the case index page markup.
 */
class AtrCaseSourceParser extends NGNodeSourceParser {
  /**
   * Clean the html right before pulling the body.
   */
  protected function cleanHtml() {
    parent::cleanHtml();

    $this->queryPath->find('h1')->first()->remove();
    $this->queryPath->find('h3')->first()->remove();

    HtmlCleanUp::removeWrapperElements($this->queryPath, array('.atr-bottom-left'));
    HtmlCleanUp::removeTableBackgrounds($this->queryPath);
  }
}

/**
 * Class AtrCaseDocumentMigration.
 *
 * @package doj_migration
 * @subpackage atr
 */
class AtrCaseDocumentMigration extends AtrCsvMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    $this->baseDir = variable_get('doj_migration_base_dir', DOJ_LEGACY_WEBROOT);
    $source_file = __DIR__ . '/../sources/atr-case-documents.csv';
    $options = array('header_rows' => TRUE);
    parent::__construct($arguments, $source_file, $options);

    $this->dependencies = array('Organization');
    $this->description = t('Migrates ATR Case Documents.');

    // Create a map object for tracking the relationships between source rows.
    $this->map = new MigrateSQLMap($this->machineName,
      array(
        'document_url' => array(
          'type' => 'varchar',
          'length' => 255,
          'not null' => TRUE,
          'description' => 'Title',
        ),
      ),
      MigrateDestinationNode::getKeySchema()
    );

    $this->destination = new MigrateDestinationNode('case_document');

    $this->addFieldMapping('field_casedoc_body', 'body');
    $this->addFieldMapping('field_casedoc_body:format')->defaultValue('wysiwyg');
    $this->addFieldMapping('field_casedoc_date', 'date');
    $this->addFieldMapping('field_casedoc_type', 'type')->callbacks(array($this, 'mapTypeToAllowedValue'));

    $this->addFieldMapping('field_casedoc_attachments', 'attachment_filename');
    $this->addFieldMapping('field_casedoc_attachments:source_type')->defaultValue('filename');
    $this->addFieldMapping('field_casedoc_attachments:file_class')->defaultValue('MigrateFileUri');
    $this->addFieldMapping('field_casedoc_attachments:destination_dir', 'destination_dir');
    $this->addFieldMapping('field_casedoc_attachments:source_dir', 'source_dir');
    $this->addFieldMapping('field_casedoc_attachments:file_replace')->defaultValue(FILE_EXISTS_REUSE);

    $this->addFieldMapping('context')->defaultValue('no_right_sidebar');
  }

  /**
   * Maps a type value to the corresponding select list key.
   *
   * @param string $value
   *   The value from the source.
   *
   * @return string
   *   The corresponding select list key, or else the raw value.
   */
  protected function mapTypeToAllowedValue($value) {
    if ($key = $this->mapValueToSelectListKey('field_casedoc_type', $value)) {
      return $key;
    }

    $this->migrationMessage("Could not find allowed value for @field_name, source value was '@value',", array(
      '@field_name' => 'field_casedoc_type',
      '@value' => $value,
    ), WATCHDOG_WARNING);

    return $value;
  }

  /**
   * {@inheritdoc}
   */
  public function csvcolumns() {
    return $this->caseDocumentCsvColumns();
  }

  /**
   * {@inheritdoc}
   */
  public function fields() {
    // This is used by the migration analysis tool and is not necessary
    // for a functioning migration.
    return array();
  }

  /**
   * {@inheritdoc}
   */
  public function prepareRow($row) {

    // Get more information about the linked file.
    $current_dir = dirname($row->document_url);
    $row->url_info = $this->getUrlInfo($row->document_url, $current_dir);
    $row->legacy_path = $row->url_path = $row->url_info['relative_url'];

    switch ($row->url_info['extension']) {
      // If case document is HTML, parse and populate node body.
      case "htm":
      case "html":

        try {
          $row->filedata = file_get_contents($row->url_info['system_path']);
        }
        catch (Exception $e) {
          $this->migrationMessage("Failed to load file '@file' referenced in '@parent'.", array(
            '@file' => $row->url_info['system_path'],
            '@parent' => $row->index_url,
          ), WATCHDOG_ERROR);

          return FALSE;
        }

        $this->sourceParser = new AtrCaseDocumentSourceParser($row->url_info['relative_url'], $row->filedata);
        $row->body = $this->sourceParser->getBody();

        // Attach pdf if present.
        $pdf_links = $this->sourceParser->queryPath->find('a')->filterPreg('/PDF/');
        foreach ($pdf_links as $pdf_link) {
          $pdf_url = $pdf_link->attr('href');
          $url_info = $this->getUrlInfo(trim($pdf_url), $this->url_info['directory']);
          $row->source_dir[] = $url_info['system_dir'];
          $row->attachment_filename[] = $url_info['filename'];
          $row->attachmentRedirects[] = $url_info['relative_url'];
        }

        // Attach wpd links if present.
        $wpd_links = $this->sourceParser->queryPath->find('a')->filterPreg('/WordPerfect/');
        foreach ($wpd_links as $wpd_link) {
          $url_info = $this->getUrlInfo(trim($wpd_link->attr('href')), $this->url_info['directory']);
          $row->source_dir[] = $url_info['system_dir'];
          $row->attachment_filename[] = $url_info['filename'];
          $row->attachmentRedirects[] = $url_info['relative_url'];
        }

        break;

      // If case document is a pdf, attach it to the node.
      case "pdf":
        $row->source_dir = $row->url_info['system_dir'];
        $row->attachment_filename = $row->url_info['filename'];

        // We don't create a redirect for the attachment. Instead, we point
        // the legacy url to the new node via $row->legacy_path.
        // $row->attachmentRedirects = array($row->url_path);

        break;
    }

    // Generate filepath based on case document date.
    $row->time_created = strtotime($row->date);
    $year_created = date('Y', $row->time_created);
    $month_created = date('m', $row->time_created);
    $date_created = date('d', $row->time_created);
    $row->destination_dir = "public:///atr/case-documents/attachments/$year_created/$month_created/$date_created";

    $row->uid = 1;

    // Run workbench functions in parent class.
    parent::prepareRow($row);

  }

  /**
   * {@inheritdoc}
   */
  public function complete($entity, $row) {
    parent::complete($entity, $row);

    if (module_exists('redirect') && !empty($row->attachmentRedirects)) {
      $this->createAttachmentRedirect($entity, $row->attachmentRedirects, 'field_casedoc_attachments');
    }
  }

  /**
   * {@inheritdoc}
   */
  public function prepareRollback($entity_id) {
    $entities = entity_load('node', $entity_id);
    $entity = reset($entities);

    $this->rollbackAttachmentRedirect($entity, 'field_casedoc_attachments');
  }

  /**
   * Provides detailed information on a URL.
   *
   * URLs may come in a variety of absolute or relative forms. E.g.,
   *  - http://justice.gov/atr/cases/f238200/238245.htm
   *  - /atr/cases/f238200/238245.htm
   *  - /./f238200/238245.htm
   *  - f238200/238245.htm
   *  - http://www.gpo.gov/fdsys/pkg/FR-2014-05-20/pdf/2014-11577.pdf
   *  - /doj/repo/legacy/www.justice.gov/atr/cases/f258800/258892.htm
   *
   * @param string $url
   *   The URL for which to provide information.
   *
   * @param string $current_dir
   *   The current directory of the file containing the link. This is used
   *   to determine absolute path when the URL is relative. E.g., 'atr/cases'.
   *
   * @return array
   *   An array of properties of the provided URL.
   *
   * @todo Create public gist of this function.
   */
  protected function getUrlInfo($url, $current_dir = '', $base_url = 'http://www.justice.gov') {
    $parsed_url = parse_url($url);
    $parsed_base_url = parse_url($base_url);

    $info = array();
    $info['original'] = $url;
    $info['extension'] = pathinfo($url, PATHINFO_EXTENSION);
    $info['filename'] = basename($url);

    $info['is_relative'] = empty($parsed_url['scheme']) && !empty($parsed_url['path']) && (substr($parsed_url['path'], 0, 1) !== '/' || substr($parsed_url['path'], 0, 3) == '/./');
    $info['external'] = !empty($parsed_url['host']) && !($parsed_base_url['host'] == $parsed_url['host']);

    if ($info['is_relative'] && $current_dir) {
      $info['directory'] = trim($current_dir, '/') . '/' . trim(dirname($url), '/');
    }
    else {
      $info['directory'] = ltrim(dirname($parsed_url['path']), '/');
    }
    // Remove occurrences of "/./" with "/".
    $info['directory'] = str_replace('/./', '/', $info['directory']);

    $info['absolute_url'] = url($info['directory'] . '/' . $info['filename'], array('absolute' => TRUE, 'base_url' => $base_url));
    $info['relative_url'] = ltrim(url($info['directory'] . '/' . $info['filename']), '/');

    if (!$info['external']) {
      // If the base path was passed in the original URL,
      if (strstr($info['absolute_url'], ltrim($this->baseDir, '/')) === FALSE) {
        $info['system_path'] = $this->baseDir . '/' . $info['directory'] . '/' . $info['filename'];
      }
      else {
        $info['system_path'] = '/' . $info['directory'] . '/' . $info['filename'];
      }

      $info['system_dir'] = dirname($info['system_path']);
    }
    else {
      $info['system_path'] = NULL;
      $info['system_dir'] = NULL;
    }

    return $info;
  }
}

/**
 * This sourceparser is used parse the actual speech.
 */
class AtrCaseDocumentSourceParser extends NGNodeSourceParser {

  /**
   * Clean the html right before pulling the body.
   */
  protected function cleanHtml() {
    parent::cleanHtml();
    HtmlCleanUp::removeTableBackgrounds($this->queryPath);
  }
}

/**
 * SourceParser for /atr pages.
 *
 * @package doj_migration
 * @subpackage atr
 */
class AtrPageSourceParser extends NGNodeSourceParser {
  /**
   * {@inheritdoc}
   */
  protected function cleanHtml() {
    parent::cleanHtml();
  }
}

/**
 * SourceParser for /atr press releases.
 *
 * @package doj_migration
 * @subpackage atr
 */
class AtrPressSourceParser extends NGPressReleaseSourceParser {

  /**
   * @var array
   *   A flat array of file links from markup.
   */
  public $fileLinks;

  /**
   * {@inheritdoc}
   */
  protected function cleanHtml() {
    parent::cleanHtml();

    $this->fileLinks = $this->findFileLinks();
    $this->removeDocumentNotice();
    HtmlCleanUp::removeTableBackgrounds($this->queryPath);
  }

  /**
   * Removes the press release notice from the header.
   */
  public function removeDocumentNotice() {
    // Remove header block.
    $table = $this->queryPath->find('table')->first();
    if ($table->attr('bgcolor') == '#c0c0c0') {
      $table->remove();
    }
  }

  /**
   * Finds all anchor links point to a pdf, wpd, or docx extension.
   *
   * @return array
   *   An array of file link URLs.
   */
  public function findFileLinks() {
    $pdf_links = array();
    $links = $this->queryPath->find('a');
    foreach ($links as $link) {
      if (preg_match('/\.(pdf|wpd|docx)/', $link->attr('href'))) {
        $pdf_links[] = $link->attr('href');
      }
    }

    return $pdf_links;
  }
}

/**
 * Obtainer for title property for /atr pages.
 *
 * @package doj_migration
 * @subpackage atr
 */
class AtrObtainTitlePage extends ObtainTitle {
  /**
   * {@inheritdoc}
   */
  public static function cleanString($text) {
    $text = parent::cleanString($text);
    $remove = array(
      'Public Workshops',
      'USDOJ: Antitrust Division',
    );
    $text = str_ireplace($remove, '', $text);

    return $text;
  }
}

/**
 * Obtainer for title property for /atr press releases.
 *
 * @package doj_migration
 * @subpackage atr
 */
class AtrObtainTitlePress extends ObtainTitlePressRelease {
}

/**
 * Obtainer for id property for /atr press releases.
 *
 * @package doj_migration
 * @subpackage atr
 */
class AtrObtainPrNumber extends ObtainId {

  /**
   * Finds last <p> tag in markup.
   *
   * @return string
   *   Found text.
   */
  protected function findLastP() {
    $element = $this->queryPath->find('p')->last();
    $this->setElementToRemove($element);
    $text = $element->text();

    if (preg_match('/([0-9]+)-([0-9]+)/', $text)) {
      return $text;
    }
  }
}

/**
 * Custom obtainer for date property for /atr.
 *
 * @package doj_migration
 * @subpackage atr
 */
class AtrObtainDate extends ObtainDate {

  /**
   * Finds date in table, after <br>.
   *
   * @return string
   *   The date!
   */
  protected function findDateInTableAfterBr() {

    $tables = $this->queryPath->find("table");
    $table_num = 2;
    foreach ($tables as $key => $table) {
      if ($key == $table_num) {
        $td = $this->pluckTableCell($table, 1, 1);
        $td->find('a')->remove();
        $text = $td->find('font')->html();
        $text = $this->trimAtBr($text, 'after');

        return $text;
      }
    }
  }
}

/**
 * Class AtrGalleryMigration.
 */
class AtrGalleryMigration extends JusticeHtmlToGalleryMigration {

  /**
   * Constructor.
   */
  public function __construct($arguments) {
    // Define source directories.
    $source_dirs = array(
      'atr/public/division-update/2011/gallery',
      'atr/public/division-update/2012/gallery',
      'atr/public/update/2010/gallery',
      'atr/public/international/gallery',
    );
    $options = array(
      'recurse' => TRUE,
    );
    $arguments['obtainer_methods'] = array(
      'title' => array(
        'pluckSelector' => array('title'),
      ),
    );

    parent::__construct($arguments, $source_dirs, $options);

    $this->addFieldMapping('og_group_ref')->defaultValue('atr')->sourceMigration('Organization');
  }

  /**
   * {@inheritdoc}
   */
  public function prepareRow($row) {
    if (parent::prepareRow($row) === FALSE) {
      return FALSE;
    }
    $title = $row->title;
    $title = str_replace("U.S. Department of Justice, Antitrust Division, ", "", $title);
    $title = str_replace(", Photo Gallery", "", $title);
    $row->title = $title;
    $row->body = "";
  }
}

/**
 * Class AtrGalleryImagesMigration.
 */
class AtrGalleryImagesMigration extends JusticeHtmlToGalleryImagesMigration {

  /**
   * Constructor.
   */
  public function __construct($arguments) {
    // Define source directories.
    $source_dirs = array(
      'atr/public/division-update/2011/gallery',
      'atr/public/division-update/2012/gallery',
      'atr/public/update/2010/gallery',
      'atr/public/international/gallery',
    );
    $options = array(
      'recurse' => TRUE,
    );
    $arguments['gallery_migration'] = "AtrGallery";
    parent::__construct($arguments, $source_dirs, $options);
  }

  /**
   * {@inheritdoc}
   */
  protected function getGallerySourceDir($row_id) {
    $pieces = explode("index.html", $row_id);
    $source = $this->baseDir . $pieces[0] . "images";
    return $source;
  }
}

/**
 * Migrates speeeches from /atr.
 *
 * @package doj_migration
 */
class AtrSpeechMigration extends NGJusticeHtmlToSpeechMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    $arguments['component_tid'] = 376;
    $arguments['source_parser_class'] = "AtrSpeechListSourceParser";

    // Define source directories.
    $source_dirs = array(
      'atr/public/speeches',
    );

    $regex = '/^speech-\d{4}\.html/i';
    $options = array('recurse' => FALSE);
    $parser = new AtrSpeechChunkParser();

    // Calling parent constructor will set $this->source and $this->map.
    parent::__construct($arguments, $source_dirs, $options, $regex, $parser);
    $this->dependencies = array('Organization');
    $this->description = t('Migrates speeches from the Antitrust Division.');
    $this->addFieldMapping('og_group_ref')
      ->defaultValue('atr')
      ->sourceMigration('Organization');
    $this->addFieldMapping('context')->defaultValue('no_right_sidebar');
  }

  /**
   * {@inheritdoc}
   */
  public function prepareRow($row) {
    $row->chunk = $row->filedata;

    // Parse the filename to extract tags and year data.
    // $row->fileid will look something like:
    // Filename-?MIGRATECHUNK?-Chunk-ID
    $fileid_parts = explode('-?MIGRATECHUNK?-', $row->fileid);
    $row->parent_file_path = $fileid_parts[0];
    $row->chunk_id = $fileid_parts[1];

    // If the chunk id is not set, then there were less than 2 chunks in the
    // file. In that case, we test to see if at least one anchor tag exists.
    // @see MigrateListFiles::getIDsFromFiles().
    if (!$row->chunk_id) {
      $row->chunk_id = $row->chunk->attr('href');
    }

    if (empty($row->chunk_id)) {
      $this->migrationMessage('No chunk id was present for chunk from file "@file"', array('@file' => $row->fileid), WATCHDOG_ERROR);
      return FALSE;
    }

    $row->title = ObtainTitle::cleanString($row->chunk->text());
    $row->speaker_name = $row->chunk->parent()->prev('h3')->text();
    $row->speaker_role = $row->chunk->parent()->prev('p > em')->text();
    $date_string = $row->chunk->parent()->prev('.news-date')->text();
    if (empty($date_string)) {
      $date = '';
    }
    else {
      $date = date('n/d/Y', strtotime($date_string));
      if (!empty($date)) {
        // Output success to show progress to aid debugging.
        $this->migrationMessage("Formatted Date: @date", array('@date' => $date), WATCHDOG_DEBUG, 2);
      }
    }
    $row->field_speech_date = $date;

    // Generate file destination path.
    $row->destination_dir = "public:///atr/speeches";
    $row->speaker = $this->createStaffProfile($row->speaker_name, $row->speaker_role, ATR_NID);

    // Process attached files.
    $similar_files = $this->getAllSimilarlyNamedFiles($row->fileid, '/atr/public/speeches');

    // Attach non-html files.
    foreach ($similar_files as $extension => $file) {
      if (!in_array($extension, array('html', 'htm'))) {
        $row->attachment_filename[] = $file['filename'];
        $row->source_dir[] = dirname($file['uri']);
      }
    }

    // The attachmentRedirect map property is an array of legacy URLs for every
    // file that is added as an attachment to this row. This property will
    // later be used in complete() to create redirects to the new file
    // locations.
    $row->attachmentRedirects = $this->getAttachedFileLegacyUrls($similar_files);

    // Identify which file should be used as canonical for this node.
    // Order of preference is htm, html, pdf.
    $canonical_source_type = (!empty($similar_files['htm'])) ? 'htm' : NULL;
    $canonical_source_type = (empty($canonical_source_type) && !empty($similar_files['html'])) ? 'html' : $canonical_source_type;
    $canonical_source_type = (empty($canonical_source_type) && !empty($similar_files['pdf'])) ? 'pdf' : $canonical_source_type;
    $row->legacy_path = $similar_files[$canonical_source_type]['legacy_uri'];

    // Process the body.
    switch ($canonical_source_type) {
      case 'htm':
      case 'html':
        $system_file_path = $similar_files[$canonical_source_type]['uri'];
        $html = file_get_contents($system_file_path);
        $page = new AtrSimpleSpeechSourceParser(str_replace($this->baseDir, '', $system_file_path), $html);
        $row->body = $page->getBody();
        break;

      default:
        break;
    }
  }

  /**
   * {@inheritdoc}
   */
  public function complete($entity, $row) {
    parent::complete($entity, $row);

    // Set the organization field for each attached file.
    foreach ($entity->field_speech_attachment[LANGUAGE_NONE] as $delta => $file) {
      $file = file_load($file['fid']);
      $file->og_group_ref[LANGUAGE_NONE][0]['target_id'] = ATR_NID;
      file_save($file);
    }
  }

  /**
   * Builds an array of legacy urls for any attached files.
   *
   * @param array $files
   *   A file type keyed array of files containing a 'legacy_uri' element.
   *
   * @return array
   *   A flat array of legacy URIs.
   */
  public function getAttachedFileLegacyUrls($files) {
    $attach = array();
    if (!empty($files) && is_array($files)) {
      foreach ($files as $type => $file) {
        if (($type !== 'htm') && ($type !== 'html')) {
          // This file should be attached.
          $attach[] = $file['legacy_uri'];
        }
      }
    }
    return $attach;
  }

  /**
   * Builds a staff_profile node if it does not already exist.
   *
   * @param string $speaker
   *   The string used as the speaker's name.
   * @param string $speaker_role
   *   The speaker's job title.
   * @param int $og_id
   *   (optional). The organic group id for the speaker. Defaults to NULL.
   *
   * @return int
   *   The nid of the staff_profile node if created or pre-existing.
   */
  public function createStaffProfile($speaker, $speaker_role, $og_id = NULL) {
    // See if the profile already exists.
    $nid = $this->lookupMostRecentNodeByFieldValue('staff_profile', 'field_staff_name', $speaker);
    if (empty($nid)) {
      // Build the staff_profile.
      $node = new StdClass();
      $node->type = 'staff_profile';
      $node->language = LANGUAGE_NONE;
      node_object_prepare($node);
      $node->uid = 1;
      $node->title = $speaker;
      $node->field_staff_position[$node->language][0]['value'] = $speaker_role;
      $node->field_staff_name[$node->language][0]['value'] = $speaker;
      if (!empty($og_id)) {
        $node->og_group_ref[$node->language][0]['target_id'] = $og_id;
      }

      $node->status = 1;
      $node->workbench_moderation_state_current = 'published';
      $node->workbench_moderation_state_new = 'published';
      $node = node_submit($node);
      node_save($node);
      doj_migration_complete_workbench_settings($node, NULL);
      $nid = $node->nid;
      $this->migrationMessage("Staff Profile Node: @nid for '@speaker' was created.", array('@nid' => $nid, '@speaker' => $speaker), WATCHDOG_NOTICE, 1);
    }

    return $nid;
  }

  /**
   * Looks up the most recent published unarchived node by name by field value.
   *
   * @param string $node_type
   *   The node bundle to look within.
   * @param string $field_name
   *   The machine name of the field whose value being searched for.
   * @param string $lookup_value
   *   The value that is being searched for.
   *
   * @return int
   *   The nid of the most recent node if found, NULL if none was found.
   */
  public function lookupMostRecentNodeByFieldValue($node_type, $field_name, $lookup_value) {
    $nid = NULL;
    if (!empty($node_type) && !empty($field_name) && !empty($lookup_value)) {
      $query = new EntityFieldQuery();
      $query->entityCondition('entity_type', 'node')
        ->entityCondition('bundle', $node_type)
        ->fieldCondition($field_name, 'value', $lookup_value, '=')
        ->fieldCondition("field_archived", "value", 0, '=')
        ->propertyCondition('status', 1)
        ->propertyOrderBy('created', 'DESC');
      $results = $query->execute();
      if (!empty($results['node'])) {
        $nids = array_keys($results['node']);
        if (!empty($nids)) {
          // At least one staff_profile exists, so set the most recent one.
          $nid = $nids[0];
        }
      }

    }
    else {
      throw new MigrateException("lookupMostRecentNodeByFieldValue('$node_type', '$field_name', '$lookup_value') can not be executed with empty params.");
    }
    return $nid;
  }
}

/**
 * This sourceparser is used parse the pages containing the list of speeches.
 */
class AtrSpeechListSourceParser extends NGSpeechSourceParser {
  /**
   * Clean the html right before pulling the body.
   */
  protected function cleanHtml() {
    parent::cleanHtml();
  }
}

/**
 * This sourceparser is used parse the actual speech.
 */
class AtrSimpleSpeechSourceParser extends SourceParser {
  /**
   * Clean the html right before pulling the body.
   */
  protected function cleanHtml() {
    parent::cleanHtml();
    HtmlCleanUp::matchTextRemoveElement($this->queryPath, 'table', 'This document is available in');
    HtmlCleanUp::matchTextRemoveElement($this->queryPath, 'b', 'DEPARTMENT OF JUSTICE');
    $this->queryPath->find('font')->removeAttr('size')->removeAttr('face');
    $this->queryPath->find('table table')->first()->remove();

    // Selectors to remove.
    $selectors = array(
      'img[alt="U.S. Department of Justice Seal"]',
    );
    HtmlCleanUp::removeElements($this->queryPath, $selectors);
    HtmlCleanUp::removeTableBackgrounds($this->queryPath);
    HtmlCleanUp::convertRelativeSrcsToAbsolute($this->queryPath, $this->fileid);
  }

  /**
   * Required by abstract.  Empty as no Obtainers are needed.
   */
  public function setDefaultObtainersInfo() {

  }
}

/**
 * Obtainer for body property for /usao-sdny speech.
 *
 * @package doj_migration
 * @subpackage usao-sdny
 * @see Obtainer.api.php
 */
class AtrSpeechObtainBody extends ObtainBody {
  /**
   * {@inheritdoc}
   */
  public static function cleanString($text) {
    $text = parent::cleanString($text);

    return $text;
  }
}

/**
 * Parses a single case year page into multiple cases.
 *
 * @package doj_migration
 * @subpackage core
 */
class AtrSpeechChunkParser extends JusticeChunkParser {

  /**
   * Sets $this->chunks.
   */
  public function setChunks() {
    $this->chunks = $this->sourceParser->queryPath->find('.atr-bottom-left p a, .atr-bottom-left ul.case li a');
    if (!$this->chunks || !$this->chunks->text()) {
      $this->chunks = array();
      $this->chunkparserMessage('Could not find chunks for JusticeChunkParser child class @class_name for file "@item_id".',
        array(
          '@class_name' => get_class(),
          '@markup' => $this->sourceParser->queryPath->html(),
          '@item_id' => $this->item_id,
        ), WATCHDOG_ERROR);
    }
  }

  /**
   * {@inheritdoc}
   */
  public function setChunkIds() {
    $chunks = $this->getChunks();
    $ids = array();

    foreach ($chunks as $chunk) {
      // The ID of each speech will be the uri of the link to a speech file.
      $id = trim($chunk->attr('href'));

      // Ignore any '/press_releases/' paths.
      // Only add complete ids.
      if (!empty($id) && stripos($id, '/press_releases/') === FALSE) {
        $ids[] = $id;
      }
    }

    return $ids;
  }

  /**
   * {@inheritdoc}
   *
   * Note that this method is not called when there is only one chunk present.
   */
  public function getChunkIDs() {
    // Note that we intentionally re-generate the chunk ids each time that this
    // is called. This is because the chunks may have been changed by a
    // call to setContent() since the last time that the ids were generated.
    return $this->setChunkIds();
  }

  /**
   * {@inheritdoc}
   */
  public function getChunk($chunk_id) {
    $error_data = 'Data could not be migrated';
    $chunks = $this->getChunks();

    if (!$chunks) {
      watchdog('doj_migration', 'No data chunks were found. Cannot search for chunk @chunk_id', array('@chunk_id' => $chunk_id), WATCHDOG_ERROR);
      return $error_data;
    }

    // If the chunk id is not set, then there were less than 2 chunks in the
    // file. In that case, we test to see if at least one anchor tag exists.
    // @see MigrateListFiles::getIDsFromFiles().
    if (!$chunk_id) {
      $chunk = $chunks->find("a")->first();
    }
    // Otherwise, use $chunk_id to find the correct anchor.
    else {
      $chunk = $chunks->find("a[href='$chunk_id']");
    }

    if (!$chunk->html() && !$chunk->text()) {
      watchdog('doj_migration', 'Data could not be migrated for chunk "@chunk_id" in file "@file". All chunks markup was: @chunks_markup', array(
        '@chunk_id' => $chunk_id,
        '@file' => $this->item_id,
        '@chunks_markup' => $chunks->html(),
      ), WATCHDOG_ERROR);

      return $error_data;
    }
    else {
      return $chunk;
    }
  }
}

/**
 * Obtainer for body property for atr pages.
 *
 * @package doj_migration
 * @subpackage atr
 * @see Obtainer.api.php
 */
class AtrObtainBody extends ObtainBody {
  /**
   * {@inheritdoc}
   */
  public static function cleanString($text) {
    $remove = array(
      '<!--#include virtual="/atr/ssi/atr-footer.ssi" -->',
      '<h1>Public Workshops</h1>',
    );
    $text = str_ireplace($remove, '', $text);
    $text = parent::cleanString($text);

    return $text;
  }
}
