package EnergyModule.Action;

import com.intellij.openapi.actionSystem.AnAction;
import com.intellij.openapi.actionSystem.AnActionEvent;
import com.intellij.openapi.actionSystem.CommonDataKeys;
import com.intellij.openapi.command.WriteCommandAction;
import com.intellij.openapi.editor.Document;
import com.intellij.openapi.editor.Editor;
import com.intellij.openapi.editor.SelectionModel;
import com.intellij.openapi.project.Project;

/**
 * Created by kd913 on 01/05/17.
 */
public class GeneratePrintCode extends AnAction {

    public void actionPerformed(final AnActionEvent event) {
        final Editor editor = event.getRequiredData(CommonDataKeys.EDITOR);
        final Project project = event.getRequiredData(CommonDataKeys.PROJECT);
        //Access document, caret, and selection
        final SelectionModel selectionModel = editor.getSelectionModel();

        //New instance of Runnable to make a replacement
        Runnable runnable = () -> {
            if (EnergyProfilerGenerator.isEnabled(project)) {
                final Document document = editor.getDocument();
                String replacementText = EnergyProfilerGenerator.className + ".printTimePoints();";
                int offsetStart  = editor.getCaretModel().getOffset();

                document.insertString(offsetStart, replacementText );
            }
        };
        //Making the replacement
        WriteCommandAction.runWriteCommandAction(project, runnable);
        selectionModel.removeSelection();

    }

}
